// Participant side of the project:
//   Composite  - Participant / Member (leaf) / Team (composite)
//   Decorator  - Roles / RolesApprover wrap a Participant
//   Chain of Responsibility - how Participant uses a WorkHandler chain
#include "doctest.h"
#include "test_helpers.h"

#include "Member.h"
#include "Roles.h"
#include "RolesApprover.h"
#include "Team.h"
#include "WorkHandler.h"

WorkHandler::~WorkHandler() {}

namespace
{
class StubHandler : public WorkHandler
{
public:
    bool result;
    int handled;
    WorkItem *lastItem;
    std::vector<WorkItem *> items;
    explicit StubHandler(bool r) : result(r), handled(0), lastItem(nullptr) { successor = nullptr; }
    void update(const Event &, WorkFlowComponent *) const override {}
    bool handleRequest(WorkItem *item) override
    {
        ++handled;
        lastItem = item;
        items.push_back(item);
        return result;
    }
    void process(WorkItem &) override {}
    WorkHandler *getSuccessor() const { return successor; }
};
}

TEST_CASE("Participant: exposes its name")
{
    MockParticipant p("Ann");
    CHECK(p.getName() == "Ann");
    CHECK(p.name() == "Ann");
}

TEST_CASE("Participant: workload counts assigned items")
{
    MockParticipant p("Ann");
    WorkItem a("a"), b("b");
    CHECK(p.getWorkLoad() == 0);
    p.assignWork(&a);
    CHECK(p.getWorkLoad() == 1);
    p.assignWork(&b);
    CHECK(p.getWorkLoad() == 2);
}

TEST_CASE("Participant: assigned work is keyed by item name, so re-assigning does not double count")
{
    MockParticipant p("Ann");
    WorkItem a("same"), b("same");
    p.assignWork(&a);
    p.assignWork(&b);
    CHECK(p.getWorkLoad() == 1);
}

TEST_CASE("Participant: has no roles unless some were granted")
{
    MockParticipant p("Ann");
    CHECK_FALSE(p.hasRole("APPROVER"));
    p.addRole("APPROVER");
    CHECK(p.hasRole("APPROVER"));
    CHECK_FALSE(p.hasRole("REVIEWER"));
}

TEST_CASE("WorkHandler: setSuccessor links the next handler in the chain")
{
    StubHandler first(false), second(true);
    first.setSuccessor(&second);
    CHECK(first.getSuccessor() == &second);
    CHECK(second.getSuccessor() == nullptr);
}

TEST_CASE("Participant::performWork: offers every assigned item to the chain")
{
    MockParticipant p("Ann");
    WorkItem a("a"), b("b");
    StubHandler handler(true);
    p.assignWork(&a);
    p.assignWork(&b);
    p.setChain(&handler);

    std::string out;
    {
        IoCapture io;
        p.performWork();
        out = io.output();
    }

    CHECK(handler.handled == 2);
    CHECK(out.empty());
}

TEST_CASE("Participant::performWork: reports items nobody could handle")
{
    MockParticipant p("Ann");
    WorkItem a("a");
    StubHandler handler(false);
    p.assignWork(&a);
    p.setChain(&handler);

    std::string out;
    {
        IoCapture io;
        p.performWork();
        out = io.output();
    }

    CHECK(handler.handled == 1);
    CHECK(contains(out, "Ann"));
    CHECK(contains(out, "nobody can handle"));
}

TEST_CASE("Participant::performWork: with no chain every item is reported as unhandled")
{
    MockParticipant p("Ann");
    WorkItem a("a"), b("b");
    p.assignWork(&a);
    p.assignWork(&b);
    p.setChain(nullptr);

    std::string out;
    {
        IoCapture io;
        p.performWork();
        out = io.output();
    }

    size_t first = out.find("nobody can handle");
    REQUIRE(first != std::string::npos);
    CHECK(out.find("nobody can handle", first + 1) != std::string::npos);
}

TEST_CASE("Participant::performWork: nothing assigned means nothing happens")
{
    MockParticipant p("Ann");
    StubHandler handler(true);
    p.setChain(&handler);

    std::string out;
    {
        IoCapture io;
        p.performWork();
        out = io.output();
    }

    CHECK(handler.handled == 0);
    CHECK(out.empty());
}

TEST_CASE("Member: getWork claims an unassigned item")
{
    Member m("Peter");
    WorkItem item("frames");
    m.assignWork(&item);
    CHECK(item.getParticipant() == nullptr);

    m.getWork();

    CHECK(item.getParticipant() == &m);
}

TEST_CASE("Member: getWork skips items that already belong to someone else")
{
    Member m("Peter");
    MockParticipant other("Other");
    WorkItem taken("taken"), free("free");
    taken.setParticipant(&other);
    m.assignWork(&taken);
    m.assignWork(&free);

    m.getWork();

    CHECK(taken.getParticipant() == &other);
    CHECK(free.getParticipant() == &m);
}

TEST_CASE("Member: getWork claims at most one item per call")
{
    Member m("Peter");
    WorkItem a("a"), b("b");
    m.assignWork(&a);
    m.assignWork(&b);

    m.getWork();

    int claimed = (a.getParticipant() == &m ? 1 : 0) + (b.getParticipant() == &m ? 1 : 0);
    CHECK(claimed == 1);
}

TEST_CASE("Member: getWork with nothing to claim does nothing")
{
    Member m("Peter");
    MockParticipant other("Other");
    WorkItem taken("taken");
    taken.setParticipant(&other);
    m.assignWork(&taken);

    CHECK_NOTHROW(m.getWork());
    CHECK(taken.getParticipant() == &other);

    Member idle("Idle");
    CHECK_NOTHROW(idle.getWork());
}

TEST_CASE("Member: doWork hands the claimed item to the chain")
{
    Member m("Peter");
    WorkItem item("frames");
    StubHandler handler(true);
    m.assignWork(&item);
    m.setChain(&handler);
    m.getWork();

    std::string out;
    {
        IoCapture io;
        m.doWork();
        out = io.output();
    }

    CHECK(handler.handled == 1);
    CHECK(handler.lastItem == &item);
    CHECK_FALSE(contains(out, "nobody can handle"));
}

TEST_CASE("Member: doWork reports when the chain cannot handle the item")
{
    Member m("Peter");
    WorkItem item("frames");
    StubHandler handler(false);
    m.assignWork(&item);
    m.setChain(&handler);
    m.getWork();

    std::string out;
    {
        IoCapture io;
        m.doWork();
        out = io.output();
    }

    CHECK(contains(out, "Peter"));
    CHECK(contains(out, "nobody can handle"));
    CHECK(contains(out, "frames"));
}

TEST_CASE("Member: doWork with no chain reports the item as unhandled")
{
    Member m("Peter");
    WorkItem item("frames");
    m.assignWork(&item);
    m.setChain(nullptr);
    m.getWork();

    std::string out;
    {
        IoCapture io;
        m.doWork();
        out = io.output();
    }

    CHECK(contains(out, "nobody can handle"));
}

TEST_CASE("Member: submitWork finishes an approved item")
{
    Member m("Peter");
    WorkItem item("frames");
    m.assignWork(&item);
    m.getWork();
    setFlags(item, st::APPROVED);

    std::string out;
    {
        IoCapture io;
        m.submitWork();
        out = io.output();
    }

    CHECK(flags(item) == st::FINISHED);
}

TEST_CASE("Member: submitWork does not finish an item that is not approved")
{
    Member m("Peter");
    WorkItem item("frames");
    m.assignWork(&item);
    m.getWork();
    setFlags(item, st::IN_PROGRESS);

    std::string out;
    {
        IoCapture io;
        m.submitWork();
        out = io.output();
    }

    CHECK(flags(item) == st::IN_PROGRESS);
    CHECK_FALSE(out.empty());
}

TEST_CASE("Team: forwards doWork, getWork and submitWork to every member")
{
    Team team("Keyframes");
    MockParticipant a("a"), b("b");
    team.addMember(&a);
    team.addMember(&b);

    team.doWork();
    team.getWork();
    team.submitWork();
    team.submitWork();

    CHECK(a.doCalls == 1);
    CHECK(b.doCalls == 1);
    CHECK(a.getCalls == 1);
    CHECK(b.getCalls == 1);
    CHECK(a.submitCalls == 2);
    CHECK(b.submitCalls == 2);
}

TEST_CASE("Team: an empty team does nothing")
{
    Team team("Empty");
    CHECK_NOTHROW(team.doWork());
    CHECK_NOTHROW(team.getWork());
    CHECK_NOTHROW(team.submitWork());
}

TEST_CASE("Team: addMember ignores nullptr")
{
    Team team("T");
    MockParticipant a("a");
    team.addMember(nullptr);
    team.addMember(&a);

    CHECK_NOTHROW(team.doWork());
    CHECK(a.doCalls == 1);
}

TEST_CASE("Team: members are keyed by name, so a same-named member replaces the old one")
{
    Team team("T");
    MockParticipant first("dup"), second("dup");
    team.addMember(&first);
    team.addMember(&second);

    team.doWork();

    CHECK(first.doCalls == 0);
    CHECK(second.doCalls == 1);
}

TEST_CASE("Team: removeMember stops forwarding to that member")
{
    Team team("T");
    MockParticipant a("a"), b("b");
    team.addMember(&a);
    team.addMember(&b);

    team.removeMember("a");
    team.doWork();

    CHECK(a.doCalls == 0);
    CHECK(b.doCalls == 1);
}

TEST_CASE("Team: removeMember of an unknown name is harmless")
{
    Team team("T");
    MockParticipant a("a");
    team.addMember(&a);
    CHECK_NOTHROW(team.removeMember("nobody"));
    team.doWork();
    CHECK(a.doCalls == 1);
}

TEST_CASE("Team: teams can contain other teams (Composite)")
{
    Team outer("outer"), inner("inner");
    MockParticipant a("a"), b("b");
    inner.addMember(&a);
    outer.addMember(&inner);
    outer.addMember(&b);

    outer.doWork();
    outer.submitWork();

    CHECK(a.doCalls == 1);
    CHECK(b.doCalls == 1);
    CHECK(a.submitCalls == 1);
    CHECK(b.submitCalls == 1);
}

TEST_CASE("Team: is a Participant, so it can be used wherever one is expected")
{
    Team team("T");
    Participant *p = &team;
    CHECK(p->getName() == "T");
}

TEST_CASE("Team + Member: a team claims and submits work through its members")
{
    Team team("Keyframes");
    Member peter("Peter"), bruce("Bruce");
    WorkItem first("first 40"), second("second 40");
    peter.assignWork(&first);
    bruce.assignWork(&second);
    team.addMember(&peter);
    team.addMember(&bruce);

    team.getWork();
    CHECK(first.getParticipant() == &peter);
    CHECK(second.getParticipant() == &bruce);

    setFlags(first, st::APPROVED);
    setFlags(second, st::APPROVED);
    {
        IoCapture io;
        team.submitWork();
    }
    CHECK(flags(first) == st::FINISHED);
    CHECK(flags(second) == st::FINISHED);
}

TEST_CASE("Roles: a decorator takes the name of the participant it wraps")
{
    MockParticipant inner("Ann");
    Roles role(&inner);
    CHECK(role.getName() == "Ann");
}

TEST_CASE("Roles: forwards work calls to the wrapped participant")
{
    MockParticipant inner("Ann");
    Roles role(&inner);

    {
        IoCapture io;
        role.doWork();
        role.getWork();
        role.submitWork();
    }

    CHECK(inner.doCalls == 1);
    CHECK(inner.getCalls == 1);
    CHECK(inner.submitCalls == 1);
}

TEST_CASE("Roles: hasRole and getWorkLoad come from the wrapped participant")
{
    MockParticipant inner("Ann");
    WorkItem item("x");
    inner.addRole("DESIGNER");
    inner.assignWork(&item);
    Roles role(&inner);

    CHECK(role.hasRole("DESIGNER"));
    CHECK_FALSE(role.hasRole("APPROVER"));
    CHECK(role.getWorkLoad() == 1);
}

TEST_CASE("RolesApprover: takes the wrapped participant's name")
{
    RolesApprover approver(new MockParticipant("Ann"));
    CHECK(approver.getName() == "Ann");
}

TEST_CASE("RolesApprover: forwards work calls to the wrapped participant")
{
    MockParticipant *inner = new MockParticipant("Ann");
    RolesApprover *approver = new RolesApprover(inner);

    {
        IoCapture io;
        approver->doWork();
        approver->getWork();
        approver->submitWork();
        approver->submitWork();
    }

    CHECK(inner->doCalls == 1);
    CHECK(inner->getCalls == 1);
    CHECK(inner->submitCalls == 2);
    delete approver;
}

TEST_CASE("RolesApprover: announces itself as an approver when it works")
{
    RolesApprover approver(new MockParticipant("Ann"));
    std::string out;
    {
        IoCapture io;
        approver.doWork();
        approver.getWork();
        approver.submitWork();
        out = io.output();
    }
    CHECK(contains(out, "APPROVER"));
    CHECK(contains(out, "Ann"));
}

TEST_CASE("RolesApprover: hasRole and getWorkLoad come from the wrapped participant")
{
    MockParticipant *inner = new MockParticipant("Ann");
    WorkItem item("x");
    inner->addRole("DESIGNER");
    inner->assignWork(&item);
    RolesApprover approver(inner);

    CHECK(approver.hasRole("DESIGNER"));
    CHECK_FALSE(approver.hasRole("PRODUCER"));
    CHECK(approver.getWorkLoad() == 1);
}

TEST_CASE("RolesApprover: decorators can be stacked")
{
    MockParticipant *inner = new MockParticipant("Ann");
    RolesApprover *outer = new RolesApprover(new RolesApprover(inner));

    {
        IoCapture io;
        outer->doWork();
    }

    CHECK(inner->doCalls == 1);
    CHECK(outer->getName() == "Ann");
    delete outer;
}

TEST_CASE("RolesApprover: a decorated participant can be used inside a Team")
{
    Team team("T");
    MockParticipant *inner = new MockParticipant("Ann");
    RolesApprover approver(inner);
    team.addMember(&approver);

    {
        IoCapture io;
        team.doWork();
    }

    CHECK(inner->doCalls == 1);
}

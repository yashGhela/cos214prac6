// Memento pattern: WorkFlowMemento (Memento), WorkFlow/WorkItem (Originators), VersionHistory (Caretaker).
#include "doctest.h"
#include "test_helpers.h"

#include "VersionHistory.h"
#include "WorkFlowMemento.h"

namespace
{
// Builds   root { a, b, sub { c } }
struct Tree
{
    WorkFlow root;
    WorkItem *a;
    WorkItem *b;
    WorkFlow *sub;
    WorkItem *c;

    Tree() : root("root"), a(new WorkItem("a")), b(new WorkItem("b")), sub(new WorkFlow("sub")), c(new WorkItem("c"))
    {
        root.addChild(a);
        root.addChild(b);
        sub->addChild(c);
        root.addChild(sub);
    }
    void setAll(const std::string &state)
    {
        setFlags(root, state);
        setFlags(*a, state);
        setFlags(*b, state);
        setFlags(*sub, state);
        setFlags(*c, state);
    }
    std::string snapshot() const
    {
        return flags(root) + "|" + flags(*a) + "|" + flags(*b) + "|" + flags(*sub) + "|" + flags(*c);
    }
};
}

TEST_CASE("Memento: a WorkItem can be restored from every possible state")
{
    std::vector<std::string> states = allStates();
    for (size_t i = 0; i < states.size(); ++i)
    {
        CAPTURE(states[i]);
        WorkItem item("item");
        setFlags(item, states[i]);

        WorkFlowMemento *m = item.createMemento();
        setFlags(item, states[i] == st::CANCELLED ? st::IN_PROGRESS : st::CANCELLED);
        item.setMemento(m);

        CHECK(flags(item) == states[i]);
        delete m;
    }
}

TEST_CASE("Memento: createMemento does not change the originator")
{
    WorkItem item("item");
    setFlags(item, st::IN_REVIEW);
    WorkFlowMemento *m = item.createMemento();
    CHECK(flags(item) == st::IN_REVIEW);
    delete m;
}

TEST_CASE("Memento: a memento is a snapshot and is not affected by later changes")
{
    WorkItem item("item");
    setFlags(item, st::IN_PROGRESS);
    WorkFlowMemento *m = item.createMemento();

    setFlags(item, st::FINISHED);
    setFlags(item, st::CANCELLED);
    item.setMemento(m);

    CHECK(flags(item) == st::IN_PROGRESS);
    delete m;
}

TEST_CASE("Memento: the same memento can be restored more than once")
{
    WorkItem item("item");
    setFlags(item, st::IN_REVIEW);
    WorkFlowMemento *m = item.createMemento();

    setFlags(item, st::CANCELLED);
    item.setMemento(m);
    CHECK(flags(item) == st::IN_REVIEW);

    setFlags(item, st::FINISHED);
    item.setMemento(m);
    CHECK(flags(item) == st::IN_REVIEW);
    delete m;
}

TEST_CASE("Memento: several mementos hold independent snapshots")
{
    WorkItem item("item");
    setFlags(item, st::CREATED);
    WorkFlowMemento *early = item.createMemento();
    setFlags(item, st::IN_PROGRESS);
    WorkFlowMemento *mid = item.createMemento();
    setFlags(item, st::FINISHED);
    WorkFlowMemento *late = item.createMemento();

    item.setMemento(early);
    CHECK(flags(item) == st::CREATED);
    item.setMemento(late);
    CHECK(flags(item) == st::FINISHED);
    item.setMemento(mid);
    CHECK(flags(item) == st::IN_PROGRESS);

    delete early;
    delete mid;
    delete late;
}

TEST_CASE("Memento: a memento from a different component has no effect")
{
    WorkItem a("a");
    WorkItem b("b");
    setFlags(a, st::FINISHED);
    WorkFlowMemento *ma = a.createMemento();

    setFlags(b, st::IN_PROGRESS);
    b.setMemento(ma);

    CHECK(flags(b) == st::IN_PROGRESS);
    delete ma;
}

TEST_CASE("Memento: a WorkFlow with no children round-trips")
{
    WorkFlow wf("wf");
    setFlags(wf, st::APPROVED);
    WorkFlowMemento *m = wf.createMemento();
    setFlags(wf, st::CANCELLED);
    wf.setMemento(m);
    CHECK(flags(wf) == st::APPROVED);
    delete m;
}

TEST_CASE("Memento: a WorkFlow memento captures the whole tree")
{
    Tree t;
    t.setAll(st::IN_PROGRESS);
    setFlags(*t.b, st::IN_REVIEW);
    setFlags(*t.c, st::APPROVED);
    std::string before = t.snapshot();

    WorkFlowMemento *m = t.root.createMemento();

    t.setAll(st::CANCELLED);
    CHECK(t.snapshot() != before);

    t.root.setMemento(m);
    CHECK(t.snapshot() == before);
    delete m;
}

TEST_CASE("Memento: nested workflows are restored, including grandchildren")
{
    Tree t;
    t.setAll(st::IN_PROGRESS);
    WorkFlowMemento *m = t.root.createMemento();

    t.setAll(st::FINISHED);
    t.root.setMemento(m);

    CHECK(flags(*t.sub) == st::IN_PROGRESS);
    CHECK(flags(*t.c) == st::IN_PROGRESS);
    delete m;
}

TEST_CASE("Memento: a sub-workflow memento only restores that branch")
{
    Tree t;
    t.setAll(st::IN_PROGRESS);
    WorkFlowMemento *m = t.sub->createMemento();

    t.setAll(st::FINISHED);
    t.sub->setMemento(m);

    CHECK(flags(*t.sub) == st::IN_PROGRESS);
    CHECK(flags(*t.c) == st::IN_PROGRESS);
    CHECK(flags(t.root) == st::FINISHED);
    CHECK(flags(*t.a) == st::FINISHED);
    delete m;
}

TEST_CASE("Memento: children added after the snapshot keep their own state")
{
    WorkFlow wf("wf");
    WorkItem *old = new WorkItem("old");
    wf.addChild(old);
    setFlags(wf, st::IN_PROGRESS);
    setFlags(*old, st::IN_PROGRESS);
    WorkFlowMemento *m = wf.createMemento();

    WorkItem *added = new WorkItem("added");
    wf.addChild(added);
    setFlags(*added, st::IN_REVIEW);
    setFlags(*old, st::FINISHED);

    wf.setMemento(m);

    CHECK(flags(*old) == st::IN_PROGRESS);
    CHECK(flags(*added) == st::IN_REVIEW);
    delete m;
}

TEST_CASE("Memento: a WorkFlow memento restores a different workflow with matching names only")
{
    WorkFlow a("alpha");
    WorkFlow b("beta");
    setFlags(a, st::FINISHED);
    WorkFlowMemento *m = a.createMemento();

    setFlags(b, st::IN_PROGRESS);
    b.setMemento(m);
    CHECK(flags(b) == st::IN_PROGRESS);
    delete m;
}

TEST_CASE("VersionHistory: an empty history has no mementos")
{
    VersionHistory h;
    CHECK(h.getMemento(0) == nullptr);
    CHECK(h.getMemento(1) == nullptr);
}

TEST_CASE("VersionHistory: getMemento returns what addMemento stored")
{
    VersionHistory h;
    WorkFlowMemento *m1 = new WorkFlowMemento();
    WorkFlowMemento *m2 = new WorkFlowMemento();
    h.addMemento(1, m1);
    h.addMemento(2, m2);

    CHECK(h.getMemento(1) == m1);
    CHECK(h.getMemento(2) == m2);
    CHECK(h.getMemento(3) == nullptr);
}

TEST_CASE("VersionHistory: version numbers need not be sequential or positive")
{
    VersionHistory h;
    WorkFlowMemento *low = new WorkFlowMemento();
    WorkFlowMemento *zero = new WorkFlowMemento();
    WorkFlowMemento *high = new WorkFlowMemento();
    h.addMemento(-5, low);
    h.addMemento(0, zero);
    h.addMemento(1000, high);

    CHECK(h.getMemento(-5) == low);
    CHECK(h.getMemento(0) == zero);
    CHECK(h.getMemento(1000) == high);
    CHECK(h.getMemento(1) == nullptr);
}

TEST_CASE("VersionHistory: stored mementos restore real state (Caretaker round trip)")
{
    VersionHistory h;
    WorkItem item("item");

    setFlags(item, st::CREATED);
    h.addMemento(1, item.createMemento());
    setFlags(item, st::IN_PROGRESS);
    h.addMemento(2, item.createMemento());
    setFlags(item, st::FINISHED);

    item.setMemento(h.getMemento(1));
    CHECK(flags(item) == st::CREATED);
    item.setMemento(h.getMemento(2));
    CHECK(flags(item) == st::IN_PROGRESS);
}

TEST_CASE("VersionHistory: it owns its mementos and destroying it frees them")
{
    VersionHistory *h = new VersionHistory();
    h->addMemento(1, new WorkFlowMemento());
    h->addMemento(2, new WorkFlowMemento());
    CHECK_NOTHROW(delete h);
}

// Visitor pattern: WorkFlowVisitor and the five concrete visitors that drive state transitions, plus the VisitorTable lookup.
#include <stdexcept>

#include "doctest.h"
#include "test_helpers.h"

#include "ApproveVisitor.h"
#include "BeginWorkVisitor.h"
#include "CancelVisitor.h"
#include "FinishVisitor.h"
#include "ReviewVisitor.h"
#include "VisitorTable.h"

TEST_CASE("Visitor: accept dispatches to the overload for the concrete type")
{
    WorkItem item("item");
    WorkFlow wf("flow");
    RecordingVisitor v;

    item.accept(&v);
    wf.accept(&v);

    REQUIRE(v.log.size() == 2);
    CHECK(v.log[0] == "WorkItem:item");
    CHECK(v.log[1] == "WorkFlow:flow");
}

TEST_CASE("Visitor: dispatch works through a WorkFlowComponent pointer")
{
    WorkItem item("item");
    WorkFlow wf("flow");
    WorkFlowComponent *a = &item;
    WorkFlowComponent *b = &wf;
    RecordingVisitor v;

    a->accept(&v);
    b->accept(&v);

    REQUIRE(v.log.size() == 2);
    CHECK(v.log[0] == "WorkItem:item");
    CHECK(v.log[1] == "WorkFlow:flow");
}

TEST_CASE_TEMPLATE("BeginWorkVisitor: created -> in progress", T, WorkItem, WorkFlow)
{
    T c("c");
    BeginWorkVisitor v;
    std::string out = run(c, v);
    CHECK(flags(c) == st::IN_PROGRESS);
    CHECK_FALSE(out.empty());
}

TEST_CASE_TEMPLATE("BeginWorkVisitor: refuses to start from any other state", T, WorkItem, WorkFlow)
{
    T c("c");
    BeginWorkVisitor v;
    std::vector<std::string> states = allStatesExcept(st::CREATED);
    for (size_t i = 0; i < states.size(); ++i)
    {
        CAPTURE(states[i]);
        setFlags(c, states[i]);
        std::string out = run(c, v);
        CHECK(flags(c) == states[i]);
        CHECK_FALSE(out.empty());
    }
}

TEST_CASE_TEMPLATE("ReviewVisitor: in progress -> in review", T, WorkItem, WorkFlow)
{
    T c("c");
    setFlags(c, st::IN_PROGRESS);
    ReviewVisitor v;
    std::string out = run(c, v);
    CHECK(flags(c) == st::IN_REVIEW);
    CHECK_FALSE(out.empty());
}

TEST_CASE_TEMPLATE("ReviewVisitor: refuses to review from any other state", T, WorkItem, WorkFlow)
{
    T c("c");
    ReviewVisitor v;
    std::vector<std::string> states = allStatesExcept(st::IN_PROGRESS);
    for (size_t i = 0; i < states.size(); ++i)
    {
        CAPTURE(states[i]);
        setFlags(c, states[i]);
        std::string out = run(c, v);
        CHECK(flags(c) == states[i]);
        CHECK_FALSE(out.empty());
    }
}

TEST_CASE_TEMPLATE("ApproveVisitor: answering A approves", T, WorkItem, WorkFlow)
{
    ApproveVisitor v;
    const char *answers[] = {"A", "a"};
    for (int i = 0; i < 2; ++i)
    {
        CAPTURE(answers[i]);
        T c("c");
        setFlags(c, st::IN_REVIEW);
        run(c, v, answers[i]);
        CHECK(flags(c) == st::APPROVED);
    }
}

TEST_CASE_TEMPLATE("ApproveVisitor: answering R rejects back to in progress", T, WorkItem, WorkFlow)
{
    ApproveVisitor v;
    const char *answers[] = {"R", "r"};
    for (int i = 0; i < 2; ++i)
    {
        CAPTURE(answers[i]);
        T c("c");
        setFlags(c, st::IN_REVIEW);
        run(c, v, answers[i]);
        CHECK(flags(c) == st::IN_PROGRESS);
    }
}

TEST_CASE_TEMPLATE("ApproveVisitor: keeps asking until it gets A or R", T, WorkItem, WorkFlow)
{
    T c("c");
    setFlags(c, st::IN_REVIEW);
    ApproveVisitor v;
    std::string out = run(c, v, "x\n7\nA\n");
    CHECK(flags(c) == st::APPROVED);
    CHECK(contains(out, "Please enter A or R"));
}

TEST_CASE_TEMPLATE("ApproveVisitor: only acts on components that are in review", T, WorkItem, WorkFlow)
{
    T c("c");
    ApproveVisitor v;
    std::vector<std::string> states = allStatesExcept(st::IN_REVIEW);
    for (size_t i = 0; i < states.size(); ++i)
    {
        CAPTURE(states[i]);
        setFlags(c, states[i]);
        std::string out = run(c, v, "A");
        CHECK(flags(c) == states[i]);
        CHECK_FALSE(out.empty());
    }
}

TEST_CASE_TEMPLATE("CancelVisitor: answering Y cancels from any live state", T, WorkItem, WorkFlow)
{
    CancelVisitor v;
    const char *live[] = {"00000", "10000", "01000", "00100"};
    for (int i = 0; i < 4; ++i)
    {
        CAPTURE(live[i]);
        T c("c");
        setFlags(c, live[i]);
        run(c, v, "Y");
        CHECK(flags(c) == st::CANCELLED);
    }
}

TEST_CASE_TEMPLATE("CancelVisitor: lowercase y also cancels", T, WorkItem, WorkFlow)
{
    T c("c");
    setFlags(c, st::IN_PROGRESS);
    CancelVisitor v;
    run(c, v, "y");
    CHECK(flags(c) == st::CANCELLED);
}

TEST_CASE_TEMPLATE("CancelVisitor: answering N leaves the component alone", T, WorkItem, WorkFlow)
{
    CancelVisitor v;
    const char *answers[] = {"N", "n"};
    for (int i = 0; i < 2; ++i)
    {
        CAPTURE(answers[i]);
        T c("c");
        setFlags(c, st::IN_PROGRESS);
        run(c, v, answers[i]);
        CHECK(flags(c) == st::IN_PROGRESS);
    }
}

TEST_CASE_TEMPLATE("CancelVisitor: keeps asking until it gets Y or N", T, WorkItem, WorkFlow)
{
    T c("c");
    setFlags(c, st::IN_PROGRESS);
    CancelVisitor v;
    std::string out = run(c, v, "maybe\n?\nY\n");
    CHECK(flags(c) == st::CANCELLED);
    CHECK(contains(out, "Please enter Y or N"));
}

TEST_CASE_TEMPLATE("CancelVisitor: finished and cancelled components cannot be cancelled", T, WorkItem, WorkFlow)
{
    T c("c");
    CancelVisitor v;
    const char *dead[] = {"00010", "00001"};
    for (int i = 0; i < 2; ++i)
    {
        CAPTURE(dead[i]);
        setFlags(c, dead[i]);
        std::string out = run(c, v, "Y");
        CHECK(flags(c) == dead[i]);
        CHECK_FALSE(out.empty());
    }
}

TEST_CASE("FinishVisitor: a WorkItem finishes only from approved")
{
    FinishVisitor v;

    WorkItem ok("ok");
    setFlags(ok, st::APPROVED);
    run(ok, v);
    CHECK(flags(ok) == st::FINISHED);

    std::vector<std::string> states = allStatesExcept(st::APPROVED);
    for (size_t i = 0; i < states.size(); ++i)
    {
        CAPTURE(states[i]);
        WorkItem item("item");
        setFlags(item, states[i]);
        std::string out = run(item, v);
        CHECK(flags(item) == states[i]);
        CHECK_FALSE(out.empty());
    }
}

TEST_CASE("FinishVisitor: a WorkFlow must itself be approved first")
{
    FinishVisitor v;
    std::vector<std::string> states = allStatesExcept(st::APPROVED);
    for (size_t i = 0; i < states.size(); ++i)
    {
        CAPTURE(states[i]);
        WorkFlow wf("wf");
        setFlags(wf, states[i]);
        std::string out = run(wf, v);
        CHECK(flags(wf) == states[i]);
        CHECK_FALSE(out.empty());
    }
}

TEST_CASE("FinishVisitor: an approved WorkFlow with no children finishes")
{
    WorkFlow wf("wf");
    setFlags(wf, st::APPROVED);
    FinishVisitor v;
    run(wf, v);
    CHECK(flags(wf) == st::FINISHED);
}

TEST_CASE("FinishVisitor: an approved WorkFlow waits for unapproved children")
{
    FinishVisitor v;
    const char *blocking[] = {"00000", "10000", "01000"};
    for (int i = 0; i < 3; ++i)
    {
        CAPTURE(blocking[i]);
        WorkFlow wf("wf");
        WorkItem *kid = new WorkItem("kid");
        wf.addChild(kid);
        setFlags(wf, st::APPROVED);
        setFlags(*kid, blocking[i]);

        std::string out = run(wf, v);
        CHECK(flags(wf) == st::APPROVED);
        CHECK_FALSE(out.empty());
    }
}

TEST_CASE("FinishVisitor: an approved WorkFlow finishes when children are approved, finished or cancelled")
{
    FinishVisitor v;
    const char *settled[] = {"00100", "00010", "00001"};
    for (int i = 0; i < 3; ++i)
    {
        CAPTURE(settled[i]);
        WorkFlow wf("wf");
        WorkItem *kid = new WorkItem("kid");
        wf.addChild(kid);
        setFlags(wf, st::APPROVED);
        setFlags(*kid, settled[i]);

        run(wf, v);
        CHECK(flags(wf) == st::FINISHED);
    }
}

TEST_CASE("FinishVisitor: one unsettled child out of several is enough to block")
{
    WorkFlow wf("wf");
    WorkItem *done = new WorkItem("done");
    WorkItem *late = new WorkItem("late");
    wf.addChild(done);
    wf.addChild(late);
    setFlags(wf, st::APPROVED);
    setFlags(*done, st::FINISHED);
    setFlags(*late, st::IN_PROGRESS);

    FinishVisitor v;
    run(wf, v);
    CHECK(flags(wf) == st::APPROVED);

    setFlags(*late, st::CANCELLED);
    run(wf, v);
    CHECK(flags(wf) == st::FINISHED);
}

TEST_CASE("Visitors: applying a visitor to a WorkFlow does not cascade to its children")
{
    WorkFlow wf("wf");
    WorkItem *kid = new WorkItem("kid");
    wf.addChild(kid);

    BeginWorkVisitor begin;
    run(wf, begin);
    CHECK(flags(wf) == st::IN_PROGRESS);
    CHECK(flags(*kid) == st::CREATED);

    CancelVisitor cancel;
    run(wf, cancel, "Y");
    CHECK(flags(wf) == st::CANCELLED);
    CHECK(flags(*kid) == st::CREATED);
}

TEST_CASE("Visitors: a full lifecycle walks the whole state machine")
{
    WorkItem item("item");
    BeginWorkVisitor begin;
    ReviewVisitor review;
    ApproveVisitor approve;
    FinishVisitor finish;

    CHECK(flags(item) == st::CREATED);
    run(item, begin);
    CHECK(flags(item) == st::IN_PROGRESS);
    run(item, review);
    CHECK(flags(item) == st::IN_REVIEW);
    run(item, approve, "R");
    CHECK(flags(item) == st::IN_PROGRESS);
    run(item, review);
    run(item, approve, "A");
    CHECK(flags(item) == st::APPROVED);
    run(item, finish);
    CHECK(flags(item) == st::FINISHED);
}

TEST_CASE("VisitorTable: each action maps to its visitor")
{
    CHECK(dynamic_cast<BeginWorkVisitor *>(&visitorFor(ActionRegistry::BeginWork)) != nullptr);
    CHECK(dynamic_cast<ReviewVisitor *>(&visitorFor(ActionRegistry::InReview)) != nullptr);
    CHECK(dynamic_cast<ApproveVisitor *>(&visitorFor(ActionRegistry::Approved)) != nullptr);
    CHECK(dynamic_cast<CancelVisitor *>(&visitorFor(ActionRegistry::Cancel)) != nullptr);
    CHECK(dynamic_cast<FinishVisitor *>(&visitorFor(ActionRegistry::Finish)) != nullptr);
}

TEST_CASE("VisitorTable: returns the same shared visitor on every call")
{
    CHECK(&visitorFor(ActionRegistry::BeginWork) == &visitorFor(ActionRegistry::BeginWork));
    CHECK(&visitorFor(ActionRegistry::BeginWork) != &visitorFor(ActionRegistry::Finish));
}

TEST_CASE("VisitorTable: an unknown action throws")
{
    CHECK_THROWS_AS(visitorFor(static_cast<ActionRegistry>(99)), std::out_of_range);
}

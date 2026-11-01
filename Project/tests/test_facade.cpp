// Facade pattern: TIVIDYFacade
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "test_helpers.h"

#include "VersionHistory.h"
#include "WorkFlowMemento.h"

#define private public
#include "TIVIDYFacade.h"
#undef private

#include "doctest.h"

namespace
{
template <typename F>
std::string quietly(F op, const std::string &input = "")
{
    IoCapture io(input);
    op();
    return io.output();
}

WorkFlow &flow(TIVIDYFacade &f) { return *static_cast<WorkFlow *>(f.workflow); }
}

TEST_CASE("Facade: starts empty")
{
    TIVIDYFacade f;
    CHECK(f.workflow == nullptr);
    CHECK(f.getVersion() == 0);
    CHECK(f.getProgress() == 0);
}

TEST_CASE("Facade: every operation is a safe no-op before a workflow exists")
{
    TIVIDYFacade f;
    std::string out;
    out += quietly([&] { f.StartWorkFlow(); });
    out += quietly([&] { f.ReviewWorkFlow(); });
    out += quietly([&] { f.CompleteWorkFlow(); });
    out += quietly([&] { f.CancelWorkFlow(); }, "Y");
    out += quietly([&] { f.saveVersion(); });
    out += quietly([&] { f.regressVersion(1); });

    CHECK(out.empty());
    CHECK(f.workflow == nullptr);
    CHECK(f.getVersion() == 0);
    CHECK(f.getProgress() == 0);
}

TEST_CASE("Facade: CreateWorkFlow makes a named workflow in the created state")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("Animation"); });

    REQUIRE(f.workflow != nullptr);
    CHECK(f.workflow->getName() == "Animation");
    CHECK(f.workflow->isCreated());
}

TEST_CASE("Facade: CreateWorkFlow replaces any previous workflow")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("First"); });
    quietly([&] { f.StartWorkFlow(); });
    quietly([&] { f.CreateWorkFlow("Second"); });

    CHECK(f.workflow->getName() == "Second");
    CHECK(f.workflow->isCreated());
}

TEST_CASE("Facade: StartWorkFlow moves the workflow to in progress")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });
    quietly([&] { f.StartWorkFlow(); });
    CHECK(flags(*f.workflow) == st::IN_PROGRESS);
}

TEST_CASE("Facade: ReviewWorkFlow moves an in-progress workflow to review")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });
    quietly([&] { f.StartWorkFlow(); });
    quietly([&] { f.ReviewWorkFlow(); });
    CHECK(flags(*f.workflow) == st::IN_REVIEW);
}

TEST_CASE("Facade: ReviewWorkFlow before starting is refused")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });
    quietly([&] { f.ReviewWorkFlow(); });
    CHECK(flags(*f.workflow) == st::CREATED);
}

TEST_CASE("Facade: CompleteWorkFlow finishes an approved workflow")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });
    setFlags(*f.workflow, st::APPROVED);
    quietly([&] { f.CompleteWorkFlow(); });
    CHECK(flags(*f.workflow) == st::FINISHED);
}

TEST_CASE("Facade: CompleteWorkFlow refuses a workflow that is not approved")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });
    quietly([&] { f.StartWorkFlow(); });
    quietly([&] { f.CompleteWorkFlow(); });
    CHECK(flags(*f.workflow) == st::IN_PROGRESS);
}

TEST_CASE("Facade: CompleteWorkFlow waits for unapproved children")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });
    WorkItem *kid = new WorkItem("kid");
    flow(f).addChild(kid);
    setFlags(*f.workflow, st::APPROVED);
    setFlags(*kid, st::IN_PROGRESS);

    quietly([&] { f.CompleteWorkFlow(); });
    CHECK(flags(*f.workflow) == st::APPROVED);

    setFlags(*kid, st::APPROVED);
    quietly([&] { f.CompleteWorkFlow(); });
    CHECK(flags(*f.workflow) == st::FINISHED);
}

TEST_CASE("Facade: CancelWorkFlow cancels after confirmation")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });
    quietly([&] { f.StartWorkFlow(); });

    quietly([&] { f.CancelWorkFlow(); }, "N");
    CHECK(flags(*f.workflow) == st::IN_PROGRESS);

    quietly([&] { f.CancelWorkFlow(); }, "Y");
    CHECK(flags(*f.workflow) == st::CANCELLED);
}

TEST_CASE("Facade: RejectWorkFlow is not implemented yet, but is safe to call")
{
    TIVIDYFacade f;
    CHECK_NOTHROW(f.RejectWorkFlow());

    quietly([&] { f.CreateWorkFlow("W"); });
    quietly([&] { f.StartWorkFlow(); });
    CHECK_NOTHROW(f.RejectWorkFlow());
    CHECK(flags(*f.workflow) == st::IN_PROGRESS);
}

TEST_CASE("Facade: operations print a message for the user")
{
    TIVIDYFacade f;
    CHECK_FALSE(quietly([&] { f.CreateWorkFlow("W"); }).empty());
    CHECK_FALSE(quietly([&] { f.StartWorkFlow(); }).empty());
    CHECK_FALSE(quietly([&] { f.ReviewWorkFlow(); }).empty());
    CHECK_FALSE(quietly([&] { f.CompleteWorkFlow(); }).empty());
    CHECK_FALSE(quietly([&] { f.CancelWorkFlow(); }, "Y").empty());
    CHECK_FALSE(quietly([&] { f.saveVersion(); }).empty());
}

TEST_CASE("Facade: AddParticipant stores participants and ignores nullptr")
{
    TIVIDYFacade f;
    MockParticipant a("a"), b("b");

    quietly([&] { f.AddParticipant(nullptr); });
    CHECK(f.participants.empty());

    quietly([&] { f.AddParticipant(&a); });
    quietly([&] { f.AddParticipant(&b); });
    REQUIRE(f.participants.size() == 2);
    CHECK(f.participants[0] == &a);
    CHECK(f.participants[1] == &b);
}

TEST_CASE("Facade: progress is the percentage of children that are finished")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });
    CHECK(f.getProgress() == 0);

    WorkItem *a = new WorkItem("a");
    WorkItem *b = new WorkItem("b");
    WorkItem *c = new WorkItem("c");
    WorkItem *d = new WorkItem("d");
    flow(f).addChild(a);
    flow(f).addChild(b);
    flow(f).addChild(c);
    flow(f).addChild(d);

    CHECK(f.getProgress() == 0);
    setFlags(*a, st::FINISHED);
    CHECK(f.getProgress() == 25);
    setFlags(*b, st::FINISHED);
    CHECK(f.getProgress() == 50);
    setFlags(*c, st::FINISHED);
    setFlags(*d, st::FINISHED);
    CHECK(f.getProgress() == 100);
}

TEST_CASE("Facade: only finished children count as progress")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });
    std::vector<std::string> notDone = allStatesExcept(st::FINISHED);
    notDone.erase(std::find(notDone.begin(), notDone.end(), st::CANCELLED));

    for (size_t i = 0; i < notDone.size(); ++i)
    {
        CAPTURE(notDone[i]);
        TIVIDYFacade g;
        quietly([&] { g.CreateWorkFlow("W"); });
        WorkItem *kid = new WorkItem("kid");
        flow(g).addChild(kid);
        setFlags(*kid, notDone[i]);
        CHECK(g.getProgress() == 0);
    }
}

TEST_CASE("Facade: cancelled children are left out of the progress calculation")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });
    WorkItem *done = new WorkItem("done");
    WorkItem *cancelled = new WorkItem("cancelled");
    flow(f).addChild(done);
    flow(f).addChild(cancelled);
    setFlags(*done, st::FINISHED);
    setFlags(*cancelled, st::CANCELLED);

    CHECK(f.getProgress() == 100);

    WorkItem *pending = new WorkItem("pending");
    flow(f).addChild(pending);
    CHECK(f.getProgress() == 50);
}

TEST_CASE("Facade: progress rounds down")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });
    WorkItem *a = new WorkItem("a");
    flow(f).addChild(a);
    flow(f).addChild(new WorkItem("b"));
    flow(f).addChild(new WorkItem("c"));
    setFlags(*a, st::FINISHED);
    CHECK(f.getProgress() == 33);
}

TEST_CASE("Facade: saveVersion numbers versions 1, 2, 3...")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });
    CHECK(f.getVersion() == 0);
    quietly([&] { f.saveVersion(); });
    CHECK(f.getVersion() == 1);
    quietly([&] { f.saveVersion(); });
    CHECK(f.getVersion() == 2);
    quietly([&] { f.saveVersion(); });
    CHECK(f.getVersion() == 3);
}

TEST_CASE("Facade: saveVersion stores a memento in the history")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });
    CHECK(f.history->getMemento(1) == nullptr);
    quietly([&] { f.saveVersion(); });
    CHECK(f.history->getMemento(1) != nullptr);
    CHECK(f.history->getMemento(2) == nullptr);
}

TEST_CASE("Facade: regressVersion restores the workflow as it was when saved")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });

    quietly([&] { f.saveVersion(); });
    quietly([&] { f.StartWorkFlow(); });
    quietly([&] { f.saveVersion(); });
    quietly([&] { f.ReviewWorkFlow(); });
    CHECK(flags(*f.workflow) == st::IN_REVIEW);

    quietly([&] { f.regressVersion(1); });
    CHECK(flags(*f.workflow) == st::CREATED);
    CHECK(f.getVersion() == 1);

    quietly([&] { f.regressVersion(2); });
    CHECK(flags(*f.workflow) == st::IN_PROGRESS);
    CHECK(f.getVersion() == 2);
}

TEST_CASE("Facade: regressVersion restores children too")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });
    WorkItem *a = new WorkItem("a");
    WorkItem *b = new WorkItem("b");
    flow(f).addChild(a);
    flow(f).addChild(b);
    setFlags(*a, st::IN_PROGRESS);
    setFlags(*b, st::IN_REVIEW);

    quietly([&] { f.saveVersion(); });

    setFlags(*a, st::FINISHED);
    setFlags(*b, st::CANCELLED);
    quietly([&] { f.regressVersion(1); });

    CHECK(flags(*a) == st::IN_PROGRESS);
    CHECK(flags(*b) == st::IN_REVIEW);
}

TEST_CASE("Facade: regressVersion to a version that does not exist changes nothing")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });
    quietly([&] { f.StartWorkFlow(); });
    quietly([&] { f.saveVersion(); });
    quietly([&] { f.ReviewWorkFlow(); });

    std::string out = quietly([&] { f.regressVersion(99); });
    quietly([&] { f.regressVersion(0); });
    quietly([&] { f.regressVersion(-1); });

    CHECK(flags(*f.workflow) == st::IN_REVIEW);
    CHECK(f.getVersion() == 1);
    CHECK(out.empty());
}

TEST_CASE("Facade: progress after regressing reflects the restored state")
{
    TIVIDYFacade f;
    quietly([&] { f.CreateWorkFlow("W"); });
    WorkItem *a = new WorkItem("a");
    WorkItem *b = new WorkItem("b");
    flow(f).addChild(a);
    flow(f).addChild(b);

    quietly([&] { f.saveVersion(); });
    setFlags(*a, st::FINISHED);
    setFlags(*b, st::FINISHED);
    CHECK(f.getProgress() == 100);

    quietly([&] { f.regressVersion(1); });
    CHECK(f.getProgress() == 0);
}

TEST_CASE("Facade: the destructor releases the workflow and history")
{
    TIVIDYFacade *f = new TIVIDYFacade();
    quietly([&] { f->CreateWorkFlow("W"); });
    flow(*f).addChild(new WorkItem("a"));
    quietly([&] { f->saveVersion(); });
    CHECK_NOTHROW(delete f);
}

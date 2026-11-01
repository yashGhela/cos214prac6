#include "doctest.h"
#include "test_helpers.h"

namespace
{
struct TrackedItem : public WorkItem
{
    static int destroyed;
    explicit TrackedItem(const std::string &n) : WorkItem(n) {}
    ~TrackedItem() { ++destroyed; }
};
int TrackedItem::destroyed = 0;
}

TEST_CASE_TEMPLATE("WorkFlowComponent: a new component is in the created state", T, WorkItem, WorkFlow)
{
    T c("c");
    CHECK(c.isCreated());
    CHECK_FALSE(c.isInProgress());
    CHECK_FALSE(c.isInReview());
    CHECK_FALSE(c.isApproved());
    CHECK_FALSE(c.isFinished());
    CHECK_FALSE(c.isCancelled());
    CHECK(flags(c) == st::CREATED);
}

TEST_CASE_TEMPLATE("WorkFlowComponent: setStatus drives the flag accessors", T, WorkItem, WorkFlow)
{
    T c("c");
    std::vector<std::string> states = allStates();
    for (size_t i = 0; i < states.size(); ++i)
    {
        CAPTURE(states[i]);
        setFlags(c, states[i]);
        CHECK(flags(c) == states[i]);
        CHECK(c.isCreated() == (states[i] == st::CREATED));
    }
}

TEST_CASE_TEMPLATE("WorkFlowComponent: getName returns the constructor name", T, WorkItem, WorkFlow)
{
    T c("Storyboard");
    CHECK(c.getName() == "Storyboard");
}

TEST_CASE("WorkItem: has no participant until one is assigned")
{
    WorkItem item("Layout");
    CHECK(item.getParticipant() == nullptr);

    MockParticipant ann("Ann");
    item.setParticipant(&ann);
    CHECK(item.getParticipant() == &ann);

    item.setParticipant(nullptr);
    CHECK(item.getParticipant() == nullptr);
}

TEST_CASE("WorkItem: is a leaf, so it has no children")
{
    WorkItem item("Layout");
    CHECK(item.getChildren().empty());
}

TEST_CASE("WorkFlow: starts with no children")
{
    WorkFlow wf("Show");
    CHECK(wf.getChildren().empty());
}

TEST_CASE("WorkFlow: addChild stores children in insertion order")
{
    WorkFlow wf("Show");
    WorkItem *a = new WorkItem("a");
    WorkItem *b = new WorkItem("b");
    WorkFlow *sub = new WorkFlow("sub");

    CHECK(wf.addChild(a));
    CHECK(wf.addChild(b));
    CHECK(wf.addChild(sub));

    std::vector<WorkFlowComponent *> kids = wf.getChildren();
    REQUIRE(kids.size() == 3);
    CHECK(kids[0] == a);
    CHECK(kids[1] == b);
    CHECK(kids[2] == sub);
}

TEST_CASE("WorkFlow: addChild rejects a child that was already added")
{
    WorkFlow wf("Show");
    WorkItem *a = new WorkItem("a");

    CHECK(wf.addChild(a));
    CHECK_FALSE(wf.addChild(a));
    CHECK(wf.getChildren().size() == 1);
}

TEST_CASE("WorkFlow: removeChild returns the child and hands ownership back")
{
    WorkFlow wf("Show");
    WorkItem *a = new WorkItem("a");
    WorkItem *b = new WorkItem("b");
    wf.addChild(a);
    wf.addChild(b);

    CHECK(wf.removeChild(a) == a);

    std::vector<WorkFlowComponent *> kids = wf.getChildren();
    REQUIRE(kids.size() == 1);
    CHECK(kids[0] == b);

    delete a;
}

TEST_CASE("WorkFlow: removeChild returns nullptr for something that is not a child")
{
    WorkFlow wf("Show");
    WorkItem stranger("stranger");
    WorkItem *a = new WorkItem("a");
    wf.addChild(a);

    CHECK(wf.removeChild(&stranger) == nullptr);
    CHECK(wf.removeChild(nullptr) == nullptr);
    CHECK(wf.getChildren().size() == 1);
}

TEST_CASE("WorkFlow: removeChild on an empty workflow is safe")
{
    WorkFlow wf("Show");
    WorkItem stranger("stranger");
    CHECK(wf.removeChild(&stranger) == nullptr);
}

TEST_CASE("WorkFlow: getChildren returns only direct children")
{
    WorkFlow wf("Show");
    WorkFlow *sub = new WorkFlow("sub");
    WorkItem *deep = new WorkItem("deep");
    sub->addChild(deep);
    wf.addChild(sub);

    std::vector<WorkFlowComponent *> kids = wf.getChildren();
    REQUIRE(kids.size() == 1);
    CHECK(kids[0] == sub);
    CHECK(sub->getChildren().size() == 1);
}

TEST_CASE("WorkFlow: a workflow can be a child of another workflow")
{
    WorkFlow root("root");
    WorkFlow *mid = new WorkFlow("mid");
    WorkFlow *leafFlow = new WorkFlow("leafFlow");
    CHECK(root.addChild(mid));
    CHECK(mid->addChild(leafFlow));
    CHECK(mid->getChildren()[0] == leafFlow);
}

TEST_CASE("WorkFlow: destroying a workflow destroys its whole subtree")
{
    TrackedItem::destroyed = 0;
    {
        WorkFlow root("root");
        root.addChild(new TrackedItem("a"));
        WorkFlow *sub = new WorkFlow("sub");
        sub->addChild(new TrackedItem("b"));
        sub->addChild(new TrackedItem("c"));
        root.addChild(sub);
    }
    CHECK(TrackedItem::destroyed == 3);
}

TEST_CASE("WorkFlow: a removed child is not destroyed with the workflow")
{
    TrackedItem::destroyed = 0;
    TrackedItem *kept = new TrackedItem("kept");
    {
        WorkFlow wf("wf");
        wf.addChild(new TrackedItem("stays"));
        wf.addChild(kept);
        CHECK(wf.removeChild(kept) == kept);
    }
    CHECK(TrackedItem::destroyed == 1);
    delete kept;
    CHECK(TrackedItem::destroyed == 2);
}

TEST_CASE("Composite: the same code can treat leaves and composites uniformly")
{
    WorkItem item("item");
    WorkFlow wf("wf");
    WorkFlowComponent *parts[2] = {&item, &wf};

    CHECK(parts[0]->getName() == "item");
    CHECK(parts[1]->getName() == "wf");
    CHECK(parts[0]->getChildren().empty());
    CHECK(parts[1]->getChildren().empty());
}

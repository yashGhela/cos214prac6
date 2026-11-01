// Iterator pattern: Aggregate / Iterator, WorkFlowIterator, VersionIterator.
#include <memory>

#include "doctest.h"
#include "test_helpers.h"

#include "VersionHistory.h"
#include "VersionIterator.h"
#include "WorkFlowIterator.h"

namespace
{
std::unique_ptr<WorkFlowIterator> iteratorFor(WorkFlow &wf)
{
    return std::unique_ptr<WorkFlowIterator>(static_cast<WorkFlowIterator *>(wf.createIterator()));
}
std::unique_ptr<VersionIterator> iteratorFor(VersionHistory &h)
{
    return std::unique_ptr<VersionIterator>(static_cast<VersionIterator *>(h.createIterator()));
}
}

TEST_CASE("Aggregate: WorkFlow and VersionHistory create the matching iterator")
{
    WorkFlow wf("wf");
    VersionHistory h;
    Aggregate *aw = &wf;
    Aggregate *ah = &h;

    std::unique_ptr<WorkFlowIterator> iw(dynamic_cast<WorkFlowIterator *>(aw->createIterator()));
    std::unique_ptr<VersionIterator> ih(dynamic_cast<VersionIterator *>(ah->createIterator()));

    CHECK(iw.get() != nullptr);
    CHECK(ih.get() != nullptr);
}

TEST_CASE("Aggregate: every call creates a fresh iterator")
{
    WorkFlow wf("wf");
    std::unique_ptr<WorkFlowIterator> a = iteratorFor(wf);
    std::unique_ptr<WorkFlowIterator> b = iteratorFor(wf);
    CHECK(a.get() != b.get());
}

TEST_CASE("WorkFlowIterator: an empty workflow is immediately done")
{
    WorkFlow wf("wf");
    std::unique_ptr<WorkFlowIterator> it = iteratorFor(wf);
    it->first();
    CHECK(it->isDone());
    CHECK(it->currentItem() == nullptr);
}

TEST_CASE("WorkFlowIterator: a nullptr workflow behaves like an empty one")
{
    WorkFlowIterator it(nullptr);
    CHECK(it.isDone());
    CHECK(it.currentItem() == nullptr);
}

TEST_CASE("WorkFlowIterator: visits direct children in order")
{
    WorkFlow wf("wf");
    WorkItem *a = new WorkItem("a");
    WorkItem *b = new WorkItem("b");
    WorkItem *c = new WorkItem("c");
    wf.addChild(a);
    wf.addChild(b);
    wf.addChild(c);

    std::unique_ptr<WorkFlowIterator> it = iteratorFor(wf);
    std::vector<WorkFlowComponent *> seen;
    for (it->first(); !it->isDone(); it->next())
        seen.push_back(it->currentItem());

    REQUIRE(seen.size() == 3);
    CHECK(seen[0] == a);
    CHECK(seen[1] == b);
    CHECK(seen[2] == c);
}

TEST_CASE("WorkFlowIterator: descends into sub-workflows depth first")
{
    WorkFlow wf("wf");
    WorkItem *a = new WorkItem("a");
    WorkFlow *sub = new WorkFlow("sub");
    WorkItem *b = new WorkItem("b");
    WorkFlow *deep = new WorkFlow("deep");
    WorkItem *c = new WorkItem("c");
    WorkItem *d = new WorkItem("d");
    deep->addChild(c);
    sub->addChild(b);
    sub->addChild(deep);
    wf.addChild(a);
    wf.addChild(sub);
    wf.addChild(d);

    std::unique_ptr<WorkFlowIterator> it = iteratorFor(wf);
    std::vector<std::string> names;
    for (it->first(); !it->isDone(); it->next())
        names.push_back(it->currentItem()->getName());

    REQUIRE(names.size() == 6);
    CHECK(names[0] == "a");
    CHECK(names[1] == "sub");
    CHECK(names[2] == "b");
    CHECK(names[3] == "deep");
    CHECK(names[4] == "c");
    CHECK(names[5] == "d");
}

TEST_CASE("WorkFlowIterator: does not include the workflow it was created from")
{
    WorkFlow wf("wf");
    wf.addChild(new WorkItem("a"));
    std::unique_ptr<WorkFlowIterator> it = iteratorFor(wf);
    for (it->first(); !it->isDone(); it->next())
        CHECK(it->currentItem() != &wf);
}

TEST_CASE("WorkFlowIterator: an empty sub-workflow is visited but contributes nothing further")
{
    WorkFlow wf("wf");
    WorkFlow *empty = new WorkFlow("empty");
    WorkItem *after = new WorkItem("after");
    wf.addChild(empty);
    wf.addChild(after);

    std::unique_ptr<WorkFlowIterator> it = iteratorFor(wf);
    std::vector<WorkFlowComponent *> seen;
    for (it->first(); !it->isDone(); it->next())
        seen.push_back(it->currentItem());

    REQUIRE(seen.size() == 2);
    CHECK(seen[0] == empty);
    CHECK(seen[1] == after);
}

TEST_CASE("WorkFlowIterator: first() restarts the traversal")
{
    WorkFlow wf("wf");
    WorkItem *a = new WorkItem("a");
    WorkItem *b = new WorkItem("b");
    wf.addChild(a);
    wf.addChild(b);

    std::unique_ptr<WorkFlowIterator> it = iteratorFor(wf);
    it->first();
    it->next();
    CHECK(it->currentItem() == b);

    it->first();
    CHECK(it->currentItem() == a);
}

TEST_CASE("WorkFlowIterator: next() past the end is safe and stays done")
{
    WorkFlow wf("wf");
    wf.addChild(new WorkItem("a"));

    std::unique_ptr<WorkFlowIterator> it = iteratorFor(wf);
    it->first();
    it->next();
    CHECK(it->isDone());
    CHECK(it->currentItem() == nullptr);

    it->next();
    it->next();
    CHECK(it->isDone());
    CHECK(it->currentItem() == nullptr);

    it->first();
    CHECK_FALSE(it->isDone());
}

TEST_CASE("WorkFlowIterator: items are gathered when the iterator is created")
{
    WorkFlow wf("wf");
    wf.addChild(new WorkItem("a"));

    std::unique_ptr<WorkFlowIterator> it = iteratorFor(wf);
    wf.addChild(new WorkItem("added later"));

    int count = 0;
    for (it->first(); !it->isDone(); it->next())
        ++count;
    CHECK(count == 1);
}

TEST_CASE("VersionIterator: an empty history is immediately done")
{
    VersionHistory h;
    std::unique_ptr<VersionIterator> it = iteratorFor(h);
    it->first();
    CHECK(it->isDone());
    CHECK(it->currentItem() == nullptr);
}

TEST_CASE("VersionIterator: visits mementos in version order, not insertion order")
{
    VersionHistory h;
    WorkFlowMemento *m1 = new WorkFlowMemento();
    WorkFlowMemento *m2 = new WorkFlowMemento();
    WorkFlowMemento *m3 = new WorkFlowMemento();
    h.addMemento(3, m3);
    h.addMemento(1, m1);
    h.addMemento(2, m2);

    std::unique_ptr<VersionIterator> it = iteratorFor(h);
    std::vector<WorkFlowMemento *> seen;
    for (it->first(); !it->isDone(); it->next())
        seen.push_back(it->currentItem());

    REQUIRE(seen.size() == 3);
    CHECK(seen[0] == m1);
    CHECK(seen[1] == m2);
    CHECK(seen[2] == m3);
}

TEST_CASE("VersionIterator: a new iterator is already positioned on the first memento")
{
    VersionHistory h;
    WorkFlowMemento *m1 = new WorkFlowMemento();
    h.addMemento(1, m1);
    h.addMemento(2, new WorkFlowMemento());

    std::unique_ptr<VersionIterator> it = iteratorFor(h);
    CHECK_FALSE(it->isDone());
    CHECK(it->currentItem() == m1);
}

TEST_CASE("VersionIterator: first() restarts the traversal")
{
    VersionHistory h;
    WorkFlowMemento *m1 = new WorkFlowMemento();
    h.addMemento(1, m1);
    h.addMemento(2, new WorkFlowMemento());

    std::unique_ptr<VersionIterator> it = iteratorFor(h);
    it->first();
    it->next();
    it->next();
    CHECK(it->isDone());

    it->first();
    CHECK_FALSE(it->isDone());
    CHECK(it->currentItem() == m1);
}

TEST_CASE("VersionIterator: next() past the end is safe")
{
    VersionHistory h;
    h.addMemento(1, new WorkFlowMemento());

    std::unique_ptr<VersionIterator> it = iteratorFor(h);
    it->first();
    it->next();
    CHECK(it->isDone());
    CHECK_NOTHROW(it->next());
    CHECK(it->isDone());
    CHECK(it->currentItem() == nullptr);
}

TEST_CASE("VersionIterator: yields mementos that can restore state")
{
    VersionHistory h;
    WorkItem item("item");
    setFlags(item, st::CREATED);
    h.addMemento(1, item.createMemento());
    setFlags(item, st::IN_PROGRESS);
    h.addMemento(2, item.createMemento());

    std::unique_ptr<VersionIterator> it = iteratorFor(h);
    it->first();
    item.setMemento(it->currentItem());
    CHECK(flags(item) == st::CREATED);
    it->next();
    item.setMemento(it->currentItem());
    CHECK(flags(item) == st::IN_PROGRESS);
}

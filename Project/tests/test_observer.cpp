// Observer pattern: Event, EventRegistry, EventSubject, ActionObserver
#include "doctest.h"
#include "test_helpers.h"

#include "ActionObserver.h"

TEST_CASE("Event: exposes its name and action")
{
    Event e("kitchen_closed", ActionRegistry::Cancel);
    CHECK(e.getName() == "kitchen_closed");
    CHECK(e.getAction() == ActionRegistry::Cancel);
    CHECK(e.getReactors().empty());
}

TEST_CASE("Event: addReactor keeps reactors in the order they were added")
{
    Event e("e", ActionRegistry::BeginWork);
    MockObserver a, b, c;
    e.addReactor(&a);
    e.addReactor(&b);
    e.addReactor(&c);

    const std::vector<WorkFlowObserver *> &r = e.getReactors();
    REQUIRE(r.size() == 3);
    CHECK(r[0] == &a);
    CHECK(r[1] == &b);
    CHECK(r[2] == &c);
}

TEST_CASE("Event: removeReactor removes only the given reactor")
{
    Event e("e", ActionRegistry::BeginWork);
    MockObserver a, b, c;
    e.addReactor(&a);
    e.addReactor(&b);
    e.addReactor(&c);

    e.removeReactor(&b);

    const std::vector<WorkFlowObserver *> &r = e.getReactors();
    REQUIRE(r.size() == 2);
    CHECK(r[0] == &a);
    CHECK(r[1] == &c);
}

TEST_CASE("Event: removeReactor of an unknown reactor changes nothing")
{
    Event e("e", ActionRegistry::BeginWork);
    MockObserver a, stranger;
    e.addReactor(&a);
    e.removeReactor(&stranger);
    CHECK(e.getReactors().size() == 1);
}

TEST_CASE("EventRegistry: normalise upper-cases and replaces spaces")
{
    CHECK(EventRegistry::normalise("kitchen closed") == "KITCHEN_CLOSED");
    CHECK(EventRegistry::normalise("Kitchen_Closed") == "KITCHEN_CLOSED");
    CHECK(EventRegistry::normalise("") == "");
}

TEST_CASE("EventRegistry: define registers an event once")
{
    EventRegistry reg;
    CHECK_FALSE(reg.exists("rush"));
    CHECK(reg.define("rush", ActionRegistry::BeginWork));
    CHECK(reg.exists("rush"));
    CHECK_FALSE(reg.define("rush", ActionRegistry::Cancel));
}

TEST_CASE("EventRegistry: names are case and space insensitive")
{
    EventRegistry reg;
    CHECK(reg.define("kitchen closed", ActionRegistry::Cancel));
    CHECK(reg.exists("KITCHEN_CLOSED"));
    CHECK(reg.exists("Kitchen Closed"));
    CHECK_FALSE(reg.define("KITCHEN_CLOSED", ActionRegistry::Cancel));
}

TEST_CASE("EventRegistry: all lists the normalised names")
{
    EventRegistry reg;
    CHECK(reg.all().empty());
    reg.define("rush hour", ActionRegistry::BeginWork);
    reg.define("closing", ActionRegistry::Finish);

    std::vector<std::string> names = reg.all();
    REQUIRE(names.size() == 2);
    CHECK(names[0] == "CLOSING");
    CHECK(names[1] == "RUSH_HOUR");
}

TEST_CASE("EventSubject: Attach registers an observer on the event")
{
    EventSubject subject;
    Event e("e", ActionRegistry::BeginWork);
    MockObserver obs;
    subject.Attach(e, &obs);
    REQUIRE(e.getReactors().size() == 1);
    CHECK(e.getReactors()[0] == &obs);
}

TEST_CASE("EventSubject: Detach unregisters an observer")
{
    EventSubject subject;
    Event e("e", ActionRegistry::BeginWork);
    MockObserver a, b;
    subject.Attach(e, &a);
    subject.Attach(e, &b);

    subject.Detach(e, &a);

    REQUIRE(e.getReactors().size() == 1);
    CHECK(e.getReactors()[0] == &b);
}

TEST_CASE("EventSubject: Detach of an observer that was never attached is harmless")
{
    EventSubject subject;
    Event e("e", ActionRegistry::BeginWork);
    MockObserver a, stranger;
    subject.Attach(e, &a);
    subject.Detach(e, &stranger);
    CHECK(e.getReactors().size() == 1);
}

TEST_CASE("EventSubject: notify calls every attached observer with the event and source")
{
    EventSubject subject;
    WorkItem source("source");
    Event e("e", ActionRegistry::BeginWork);
    MockObserver a, b;
    subject.Attach(e, &a);
    subject.Attach(e, &b);

    subject.notify(e, &source);

    CHECK(a.calls == 1);
    CHECK(b.calls == 1);
    CHECK(a.lastEvent == &e);
    CHECK(b.lastEvent == &e);
    CHECK(a.lastSource == &source);
    CHECK(b.lastSource == &source);
}

TEST_CASE("EventSubject: observers are notified in the order they attached")
{
    EventSubject subject;
    Event e("e", ActionRegistry::BeginWork);
    std::vector<std::string> order;
    MockObserver first, second, third;
    first.id = "1";
    second.id = "2";
    third.id = "3";
    first.orderLog = second.orderLog = third.orderLog = &order;
    subject.Attach(e, &first);
    subject.Attach(e, &second);
    subject.Attach(e, &third);

    subject.notify(e, nullptr);

    REQUIRE(order.size() == 3);
    CHECK(order[0] == "1");
    CHECK(order[1] == "2");
    CHECK(order[2] == "3");
}

TEST_CASE("EventSubject: notify with no observers does nothing")
{
    EventSubject subject;
    Event e("e", ActionRegistry::BeginWork);
    CHECK_NOTHROW(subject.notify(e, nullptr));
}

TEST_CASE("EventSubject: a detached observer is no longer notified")
{
    EventSubject subject;
    Event e("e", ActionRegistry::BeginWork);
    MockObserver a, b;
    subject.Attach(e, &a);
    subject.Attach(e, &b);
    subject.Detach(e, &a);

    subject.notify(e, nullptr);

    CHECK(a.calls == 0);
    CHECK(b.calls == 1);
}

TEST_CASE("EventSubject: observers only hear about the events they attached to")
{
    EventSubject subject;
    Event rush("rush", ActionRegistry::BeginWork);
    Event close("close", ActionRegistry::Finish);
    MockObserver onRush, onClose;
    subject.Attach(rush, &onRush);
    subject.Attach(close, &onClose);

    subject.notify(rush, nullptr);

    CHECK(onRush.calls == 1);
    CHECK(onClose.calls == 0);
}

TEST_CASE("EventSubject: an observer may detach itself while being notified")
{
    EventSubject subject;
    Event e("e", ActionRegistry::BeginWork);
    SelfDetachingObserver quitter(&subject, &e);
    MockObserver after;
    subject.Attach(e, &quitter);
    subject.Attach(e, &after);

    subject.notify(e, nullptr);

    CHECK(quitter.calls == 1);
    CHECK(after.calls == 1);
    CHECK(e.getReactors().size() == 1);

    subject.notify(e, nullptr);
    CHECK(quitter.calls == 1);
    CHECK(after.calls == 2);
}

TEST_CASE("EventSubject: attaching the same observer twice is ignored" * doctest::may_fail())
{
    EventSubject subject;
    Event e("e", ActionRegistry::BeginWork);
    MockObserver obs;
    subject.Attach(e, &obs);
    subject.Attach(e, &obs);

    CHECK(e.getReactors().size() == 1);

    subject.notify(e, nullptr);
    CHECK(obs.calls == 1);
}


namespace
{
class RecordingActionObserver : public ActionObserver
{
public:
    mutable std::vector<std::string> log;
    bool react;
    explicit RecordingActionObserver(bool r) : react(r) {}

protected:
    bool shouldReact(const Event &, WorkFlowComponent *) const override
    {
        log.push_back("shouldReact");
        return react;
    }
    void applyAction(const Event &, WorkFlowComponent *) override { log.push_back("applyAction"); }
    void afterUpdate(const Event &) override { log.push_back("afterUpdate"); }
};
}

TEST_CASE("ActionObserver: update runs shouldReact, applyAction, afterUpdate in order")
{
    RecordingActionObserver obs(true);
    Event e("e", ActionRegistry::BeginWork);
    obs.update(e, nullptr);

    REQUIRE(obs.log.size() == 3);
    CHECK(obs.log[0] == "shouldReact");
    CHECK(obs.log[1] == "applyAction");
    CHECK(obs.log[2] == "afterUpdate");
}

TEST_CASE("ActionObserver: update stops after shouldReact returns false")
{
    RecordingActionObserver obs(false);
    Event e("e", ActionRegistry::BeginWork);
    obs.update(e, nullptr);

    REQUIRE(obs.log.size() == 1);
    CHECK(obs.log[0] == "shouldReact");
}


TEST_CASE_TEMPLATE("Observer: an event drives a component through its visitor", T, WorkItem, WorkFlow)
{
    T c("c");
    Event begin("begin", ActionRegistry::BeginWork);
    c.Attach(begin, &c);

    fire(c, begin, &c);

    CHECK(flags(c) == st::IN_PROGRESS);
}

TEST_CASE_TEMPLATE("Observer: a component can be walked through its lifecycle by events", T, WorkItem, WorkFlow)
{
    T c("c");
    Event begin("begin", ActionRegistry::BeginWork);
    Event review("review", ActionRegistry::InReview);
    Event approve("approve", ActionRegistry::Approved);
    Event finish("finish", ActionRegistry::Finish);
    c.Attach(begin, &c);
    c.Attach(review, &c);
    c.Attach(approve, &c);
    c.Attach(finish, &c);

    fire(c, begin, &c);
    CHECK(flags(c) == st::IN_PROGRESS);
    fire(c, review, &c);
    CHECK(flags(c) == st::IN_REVIEW);
    fire(c, approve, &c, "A");
    CHECK(flags(c) == st::APPROVED);
    fire(c, finish, &c);
    CHECK(flags(c) == st::FINISHED);
}

TEST_CASE_TEMPLATE("Observer: a cancel event asks for confirmation", T, WorkItem, WorkFlow)
{
    T c("c");
    Event cancel("cancel", ActionRegistry::Cancel);
    c.Attach(cancel, &c);
    setFlags(c, st::IN_PROGRESS);

    fire(c, cancel, &c, "N");
    CHECK(flags(c) == st::IN_PROGRESS);

    fire(c, cancel, &c, "Y");
    CHECK(flags(c) == st::CANCELLED);
}

TEST_CASE("Observer: one event can drive many components at once")
{
    WorkFlow wf("wf");
    WorkItem *a = new WorkItem("a");
    WorkItem *b = new WorkItem("b");
    WorkItem bystander("bystander");
    wf.addChild(a);
    wf.addChild(b);

    Event begin("begin", ActionRegistry::BeginWork);
    wf.Attach(begin, &wf);
    a->Attach(begin, a);
    b->Attach(begin, b);

    fire(wf, begin, &wf);

    CHECK(flags(wf) == st::IN_PROGRESS);
    CHECK(flags(*a) == st::IN_PROGRESS);
    CHECK(flags(*b) == st::IN_PROGRESS);
    CHECK(flags(bystander) == st::CREATED);
}

TEST_CASE("Observer: a detached component stops reacting")
{
    WorkItem item("item");
    Event begin("begin", ActionRegistry::BeginWork);
    item.Attach(begin, &item);
    item.Detach(begin, &item);

    fire(item, begin, &item);

    CHECK(flags(item) == st::CREATED);
}

TEST_CASE("Observer: an invalid transition event leaves the component unchanged")
{
    WorkItem item("item");
    Event review("review", ActionRegistry::InReview);
    item.Attach(review, &item);

    fire(item, review, &item);

    CHECK(flags(item) == st::CREATED);
}

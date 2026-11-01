#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Event.h"
#include "EventSubject.h"
#include "Participant.h"
#include "WorkFlow.h"
#include "WorkFlowComponent.h"
#include "WorkFlowObserver.h"
#include "WorkFlowVisitor.h"
#include "WorkItem.h"

/* 
A component's state is five booleans: inProgress, inReview, approved,
finished, cancelled. flags() renders them as a 5 character string such as
"10000" so a whole state can be compared in one CHECK.
*/

namespace st {
static const std::string CREATED     = "00000";
static const std::string IN_PROGRESS = "10000";
static const std::string IN_REVIEW   = "01000";
static const std::string APPROVED    = "00100";
static const std::string FINISHED    = "00010";
static const std::string CANCELLED   = "00001";
}

inline std::vector<std::string> allStates()
{
    std::vector<std::string> s;
    s.push_back(st::CREATED);
    s.push_back(st::IN_PROGRESS);
    s.push_back(st::IN_REVIEW);
    s.push_back(st::APPROVED);
    s.push_back(st::FINISHED);
    s.push_back(st::CANCELLED);
    return s;
}

inline std::vector<std::string> allStatesExcept(const std::string &skip)
{
    std::vector<std::string> out;
    std::vector<std::string> all = allStates();
    for (size_t i = 0; i < all.size(); ++i)
        if (all[i] != skip)
            out.push_back(all[i]);
    return out;
}

inline std::string flags(const WorkFlowComponent &c)
{
    std::string s;
    s += c.isInProgress() ? '1' : '0';
    s += c.isInReview() ? '1' : '0';
    s += c.isApproved() ? '1' : '0';
    s += c.isFinished() ? '1' : '0';
    s += c.isCancelled() ? '1' : '0';
    return s;
}

inline void setFlags(WorkFlowComponent &c, const std::string &f)
{
    c.setStatus(f[0] == '1', f[1] == '1', f[2] == '1', f[3] == '1', f[4] == '1');
}

inline bool contains(const std::string &haystack, const std::string &needle)
{
    return haystack.find(needle) != std::string::npos;
}

class IoCapture
{
public:
    explicit IoCapture(const std::string &input = "")
        : in(input), oldOut(std::cout.rdbuf(out.rdbuf())), oldIn(std::cin.rdbuf(in.rdbuf()))
    {
        std::cin.clear();
    }
    ~IoCapture()
    {
        std::cout.rdbuf(oldOut);
        std::cin.rdbuf(oldIn);
        std::cin.clear();
    }
    std::string output() const { return out.str(); }

private:
    std::ostringstream out;
    std::istringstream in;
    std::streambuf *oldOut;
    std::streambuf *oldIn;
    IoCapture(const IoCapture &);
    IoCapture &operator=(const IoCapture &);
};

// Applies a visitor to a component with `input` available on std::cin and returns everything the visitor printed.
inline std::string run(WorkFlowComponent &c, WorkFlowVisitor &v, const std::string &input = "")
{
    IoCapture io(input);
    c.accept(&v);
    return io.output();
}

// Fires an event from a subject and returns what was printed.
inline std::string fire(EventSubject &subject, const Event &e, WorkFlowComponent *source,
                        const std::string &input = "")
{
    IoCapture io(input);
    subject.notify(e, source);
    return io.output();
}

// Observer that records how it was called.
class MockObserver : public WorkFlowObserver
{
public:
    int calls;
    const Event *lastEvent;
    WorkFlowComponent *lastSource;
    std::vector<std::string> *orderLog;
    std::string id;

    MockObserver() : calls(0), lastEvent(nullptr), lastSource(nullptr), orderLog(nullptr) {}
    void update(const Event &e, WorkFlowComponent *source) override
    {
        ++calls;
        lastEvent = &e;
        lastSource = source;
        if (orderLog)
            orderLog->push_back(id);
    }
};

// Observer that detaches itself while it is being notified.
class SelfDetachingObserver : public WorkFlowObserver
{
public:
    int calls;
    EventSubject *subject;
    Event *event;
    SelfDetachingObserver(EventSubject *s, Event *e) : calls(0), subject(s), event(e) {}
    void update(const Event &, WorkFlowComponent *) override
    {
        ++calls;
        subject->Detach(*event, this);
    }
};

// Visitor that records which overload each component dispatched to.
class RecordingVisitor : public WorkFlowVisitor
{
public:
    std::vector<std::string> log;
    void visit(WorkFlow *wf) override { log.push_back("WorkFlow:" + wf->getName()); }
    void visit(WorkItem *wi) override { log.push_back("WorkItem:" + wi->getName()); }
};

// Concrete Participant that counts calls.
class MockParticipant : public Participant
{
public:
    int doCalls, getCalls, submitCalls;
    explicit MockParticipant(const std::string &name)
        : Participant(name), doCalls(0), getCalls(0), submitCalls(0)
    {
        setChain(nullptr);
    }
    void doWork() override { ++doCalls; }
    void getWork() override { ++getCalls; }
    void submitWork() override { ++submitCalls; }
    void addRole(const std::string &r) { part_roles[r] = true; }
};

#endif

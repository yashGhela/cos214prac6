#ifndef CANCELLEDSTATE_H
#define CANCELLEDSTATE_H
#include "WorkItemState.h"
class CancelledState : public WorkItemState
{
private:
    /* data */
protected:
    WorkFlowComponent *context;

public:
    CancelledState(WorkFlowComponent *c);
    ~CancelledState();
    void review(WorkFlow *wf) override;
    void review(WorkItem *wi) override;
    void approve(WorkFlow *wf) override;
    void approve(WorkItem *wi) override;
    void cancel(WorkFlow *wf) override;
    void cancel(WorkItem *wi) override;
    void finish(WorkFlow *wf) override;
    void finish(WorkItem *wi) override;
    void beginWork(WorkFlow *wf) override;
    void beginWork(WorkItem *wi) override;
    std::string stringStatus() const override;
};

CancelledState::~CancelledState()
{
}

#endif
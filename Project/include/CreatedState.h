#ifndef CREATEDSTATE_H
#define CREATEDSTATE_H
#include "WorkItemState.h"
#include "CancelledState.h"
#include "InProgressState.h"
class CreatedState : public WorkItemState
{
private:
    /* data */
protected:
    WorkFlowComponent *context;

public:
    CreatedState(WorkFlowComponent *c);
    ~CreatedState();
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

CreatedState::~CreatedState()
{
}

#endif
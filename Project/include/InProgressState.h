#ifndef INPROGRESSSTATE_H
#define INPROGRESSSTATE_H
#include "WorkItemState.h"
#include "CancelledState.h"
#include "InReviewState.h"
class InProgressState : public WorkItemState
{
private:
    /* data */
protected:
    WorkFlowComponent *context;

public:
    InProgressState(WorkFlowComponent *c);
    ~InProgressState();
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

InProgressState::~InProgressState()
{
}

#endif
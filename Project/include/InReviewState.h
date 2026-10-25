#ifndef INREVIEWSTATE_H
#define INREIVEWSTATE_H
#include "WorkItemState.h"
#include "CancelledState.h"
#include "ApprovedState.h"
#include "InProgressState.h"
class InReviewState : public WorkItemState
{
private:
    /* data */
protected:
    WorkFlowComponent *context;

public:
    InReviewState(WorkFlowComponent *c);
    ~InReviewState();
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

InReviewState::~InReviewState()
{
}

#endif
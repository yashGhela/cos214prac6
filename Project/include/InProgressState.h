#ifndef INPROGRESSSTATE_H
#define INPROGRESSSTATE_H
#include "WorkItemState.h"
#include "CancelledState.h"
#include "InReviewState.h"
class InProgressState : public WorkItemState // The InProgress state used in a WorkFlowComponent, inheriting from the WorkItemState and implementing it.
{
private:
    /* data */
protected:
    WorkFlowComponent *context; // The WorkFlowComponent that is the context of the current State

public:
    InProgressState(WorkFlowComponent *c);     // Constructor that assigns the context of the InProgress state to the WorkFlowComponent that is passed into the function as a parameter.
    ~InProgressState();                        // Destructor.
    void review(WorkFlow *wf) override;        // Function that uses double dispatch and reviews the WorkFlow passed as a parameter if its in the correct state to do so.
    void review(WorkItem *wi) override;        // Function that uses double dispatch and reviews the WorkItem passed as a parameter if its in the correct state to do so.
    void approve(WorkFlow *wf) override;       // Function that uses double dispatch and approves the WorkFlow passed as a parameter if its in the correct state to do so.
    void approve(WorkItem *wi) override;       // Function that uses double dispatch and approves the WorkItem passed as a parameter if its in the correct state to do so.
    void cancel(WorkFlow *wf) override;        // Function that uses double dispatch and cancels the WorkFlow passed as a parameter if its in the correct state to do so.
    void cancel(WorkItem *wi) override;        // Function that uses double dispatch and cancels the WorkItem passed as a parameter if its in the correct state to do so.
    void finish(WorkFlow *wf) override;        // Function that uses double dispatch and finishes the WorkFlow passed as a parameter if its in the correct state to do so.
    void finish(WorkItem *wi) override;        // Function that uses double dispatch and finishes the WorkItem passed as a parameter if its in the correct state to do so.
    void beginWork(WorkFlow *wf) override;     // Function that uses double dispatch and begins progress the WorkFlow passed as a parameter if its in the correct state to do so.
    void beginWork(WorkItem *wi) override;     // Function that uses double dispatch and begins progress the WorkItem passed as a parameter if its in the correct state to do so.
    std::string stringStatus() const override; // Function that returns that returns a string of what the WorkItem State is represented by the current state.
};

InProgressState::~InProgressState()
{
}

#endif
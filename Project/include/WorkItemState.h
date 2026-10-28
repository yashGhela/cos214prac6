#ifndef WORKITEMSTATE_H
#define WORKITEMSTATE_H

#include <iostream>
#include "WorkFlowComponent.h"
#include "WorkFlow.h"
#include "WorkItem.h"

class WorkFlowComponent;
class WorkItem;
class WorkFlow;
class WorkItemState // An Abstract Interface for the State Pattern that the derived classes Implement.
{
private:
    /* data */
protected:
public:
    WorkItemState();                              // Constructor.
    ~WorkItemState();                             // Destructor.
    virtual void review(WorkFlow *wf) = 0;        // Pure Virtual Function that uses double dispatch and reviews the WorkFlow passed as a parameter if its in the correct state to do so.
    virtual void review(WorkItem *wi) = 0;        // Pure Virtual Function that uses double dispatch and reviews the WorkItem passed as a parameter if its in the correct state to do so.
    virtual void approve(WorkFlow *wf) = 0;       // Pure Virtual Function that uses double dispatch and approves the WorkFlow passed as a parameter if its in the correct state to do so.
    virtual void approve(WorkItem *wi) = 0;       // Pure Virtual Function that uses double dispatch and approves the WorkItem passed as a parameter if its in the correct state to do so.
    virtual void cancel(WorkFlow *wf) = 0;        // Pure Virtual Function that uses double dispatch and cancels the WorkFlow passed as a parameter if its in the correct state to do so.
    virtual void cancel(WorkItem *wi) = 0;        // Pure Virtual Function that uses double dispatch and cancels the WorkItem passed as a parameter if its in the correct state to do so.
    virtual void finish(WorkFlow *wf) = 0;        // Pure Virtual Function that uses double dispatch and finishes the WorkFlow passed as a parameter if its in the correct state to do so.
    virtual void finish(WorkItem *wi) = 0;        // Pure Virtual Function that uses double dispatch and finishes the WorkItem passed as a parameter if its in the correct state to do so.
    virtual void beginWork(WorkFlow *wf) = 0;     // Pure Virtual Function that uses double dispatch and begins progress the WorkFlow passed as a parameter if its in the correct state to do so.
    virtual void beginWork(WorkItem *wi) = 0;     // Pure Virtual Function that uses double dispatch and begins progress the WorkItem passed as a parameter if its in the correct state to do so.
    virtual std::string stringStatus() const = 0; // Pure Virtual Function that returns that returns a string of what the WorkItem State is represented by the current state.
};
WorkItemState::WorkItemState()
{
}
WorkItemState::~WorkItemState()
{
}

#endif
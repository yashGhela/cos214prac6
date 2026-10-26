#ifndef WORKITEMSTATE_H
#define WORKITEMSTATE_H

#include <iostream>
#include "WorkFlowComponent.h"
#include "WorkFlow.h"
#include "WorkItem.h"

class WorkFlowComponent;
class WorkItem;
class WorkFlow;
class WorkItemState
{
private:
    /* data */
protected:
public:
    WorkItemState();
    ~WorkItemState();
    virtual void review(WorkFlow *wf) = 0;
    virtual void review(WorkItem *wi) = 0;
    virtual void approve(WorkFlow *wf) = 0;
    virtual void approve(WorkItem *wi) = 0;
    virtual void cancel(WorkFlow *wf) = 0;
    virtual void cancel(WorkItem *wi) = 0;
    virtual void finish(WorkFlow *wf) = 0;
    virtual void finish(WorkItem *wi) = 0;
    virtual void beginWork(WorkFlow *wf) = 0;
    virtual void beginWork(WorkItem *wi) = 0;
    virtual std::string stringStatus() const = 0;
};
WorkItemState::WorkItemState()
{
}
WorkItemState::~WorkItemState()
{
}

#endif
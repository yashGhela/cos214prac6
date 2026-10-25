#ifndef WORKFLOWCOMPONENT_H
#define WORKFLOWCOMPONENT_H
#include "WorkItemState.h"
#include "WorkFlowMemento.h"
#include "Participant.h"
#include <vector>
#include <string>

class WorkFlowComponent
{
private:
public:
    WorkFlowComponent();
    ~WorkFlowComponent();
    virtual WorkFlowMemento createMemento();
    virtual void SetMemento(WorkFlowMemento memento);
    virtual void setState(WorkItemState *s) = 0;
    virtual WorkItemState *getState() = 0;
    virtual std::vector<WorkItemState *> getChildrenState() = 0;
    virtual std::string getName() = 0;
    virtual void review(WorkItemState *s) = 0;
    virtual void approve(WorkItemState *s) = 0;
    virtual void cancel(WorkItemState *s) = 0;
    virtual void finish(WorkItemState *s) = 0;
    virtual void beginWork(WorkItemState *s) = 0;
};
WorkFlowComponent::WorkFlowComponent()
{
}

WorkFlowComponent::~WorkFlowComponent()
{
}
#endif
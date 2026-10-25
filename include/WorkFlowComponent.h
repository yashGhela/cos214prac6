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
    virtual void request() = 0;
    virtual WorkFlowMemento createMemento();
    virtual void SetMemento(WorkFlowMemento memento);
    virtual void setState(WorkItemState *s) = 0;
    virtual WorkItemState *getState() = 0;
};
#endif
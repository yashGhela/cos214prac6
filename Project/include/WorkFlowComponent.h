#ifndef WORKFLOWCOMPONENT_H
#define WORKFLOWCOMPONENT_H
#include "WorkItemState.h"
#include "WorkFlowMemento.h"
#include "Participant.h"
#include <vector>
#include <string>
class WorkFlowComponent // The Abstract Interface of the WorkFlow Composite Structure.
{
private:
public:
    WorkFlowComponent();                                        // Constructor
    ~WorkFlowComponent();                                       // Destructor
    virtual WorkFlowMemento *createMemento() = 0;               // Pure virtual function to create a memento of the WorkFlowComponent.
    virtual void setMemento(WorkFlowMemento *memento) = 0;      // Pure virtual function to set a memento of the WorkFlowComponent.
    virtual void setState(WorkItemState *s) = 0;                // Pure virtual function to set state of the WorkFlowComponent.
    virtual WorkItemState *getState() = 0;                      // Pure virtual function to get the current state of the WorkFlowComponent.
    virtual std::vector<WorkFlowComponent *> getChildren() = 0; // Pure virtual function to get the children stored in the WorkFlowComponent. Gets overriden differently for a leaf and a composite.
    virtual std::string getName() = 0;                          // Pure virtual function to get the name of the WorkFlowComponent.
    virtual void review(WorkItemState *s) = 0;                  // Pure virtual function to attempt to set the state to InReview of the WorkFlowComponent.
    virtual void approve(WorkItemState *s) = 0;                 // Pure virtual function to attempt to set the state to Approved or back to InProgress of the WorkFlowComponent, depending on the choice made by the reviewer.
    virtual void cancel(WorkItemState *s) = 0;                  // Pure virtual function to attempt to set the state to Cancelled of the WorkFlowComponent.
    virtual void finish(WorkItemState *s) = 0;                  // Pure virtual function to attempt to set the state to Finished of the WorkFlowComponent.
    virtual void beginWork(WorkItemState *s) = 0;               // Pure virtual function to attempt to set the state to InProgress of the WorkFlowComponent.
};
WorkFlowComponent::WorkFlowComponent()
{
}

WorkFlowComponent::~WorkFlowComponent()
{
}
#endif
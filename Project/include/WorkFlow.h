#ifndef WORKFLOW_H
#define WORKFLOW_H
#include "WorkFlowComponent.h"
#include "Aggregate.h"
class WorkFlow : public WorkFlowComponent, public Aggregate // A class publically derived from WorkFlowComponent and Aggregate. It Implements all functions defined in the respective interfaces. It acts as a composite in the WorkFlow Composite structure.
{
private:
    WorkItemState *state;                      // The current state of a WorkFlow at any time.
    std::string name;                          // The name of the WorkFlow.
    std::vector<WorkFlowComponent *> children; // The children that the WorkFlow holds. It can contain other WorkFlows and WorkItems.

public:
    WorkFlow(std::string n);                                 // Constructor for a WorkFlow. Takes a string as a parameter to assign as its name.
    ~WorkFlow();                                             // Destructor for a Workflow, recursively deleting the children of a composite when called.
    bool addChild(WorkFlowComponent *c);                     // Adds a child of type WorkFlowComponent to the children vector of the WorkFlow. Returns true if added and false if the child is already added.
    WorkFlowComponent *removeChild(WorkFlowComponent *c);    // Removes a child of type WorkFlowComponent to the children vector of the WorkFlow and returns it if found, otherwise it returns a nullptr.
    WorkFlowMemento *createMemento() override;               // Creates a memento of the WorkFlow and returns it, storing all current children and the state the workflow is currently in.
    void setMemento(WorkFlowMemento *m) override;            // Sets a memento for the WorkFlow, setting current state and children to what was stored in the memento that is passed as a parameter.
    void setState(WorkItemState *s) override;                // Sets current state of the WorkFlow to the state that is passed as a parameter to the function.
    WorkItemState *getState() override;                      // Returns the current state of the WorkFlow.
    Iterator *createIterator();                              // Creates an iterator for the WorkFlow so that its children can be iterated through.
    std::vector<WorkFlowComponent *> getChildren() override; // Returns all children of a WorkFlow, traversing the tree-structure recursively.
    std::string getName() override;                          // Returns the name of the WorkFlow.
    void review(WorkItemState *s) override;                  // A Function using double dispatch to attempt to set the state to InReview of the WorkFlow. It will return a simple cout message if the state is not currently set to InProgress.
    void approve(WorkItemState *s) override;                 // A Function using double dispatch to attempt to set the state to Approved or back to InProgress of the WorkFlow, through a choice of the reviewer. It will return a simple cout message if the state is not currently set to InReview.
    void cancel(WorkItemState *s) override;                  // A Function using double dispatch to set the state to Cancelled of the WorkFlow, using a confirmation by the user.
    void finish(WorkItemState *s) override;                  // A Function using double dispatch to attempt to set the state to Finished of the WorkFlow, working only if all children of the composite are in a state of either Approved, Cancelled or Finished. It will return a simple cout message if the state of the WorkFlow is not currently set to Approved.
    void beginWork(WorkItemState *s) override;               // A Function using double dispatch to attempt to set the state to InProgress of the WorkFlow. It will return a simple cout message if the state is not currently set to Created.
};

#endif
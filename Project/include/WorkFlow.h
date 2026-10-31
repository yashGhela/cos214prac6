#ifndef WORKFLOW_H
#define WORKFLOW_H
#include "WorkFlowComponent.h"
#include "Aggregate.h"
class WorkFlow : public WorkFlowComponent, public Aggregate // A class publically derived from WorkFlowComponent and Aggregate. It Implements all functions defined in the respective interfaces. It acts as a composite in the WorkFlow Composite structure.
{
private:
    std::string name;                          // The name of the WorkFlow.
    std::vector<WorkFlowComponent *> children; // The children that the WorkFlow holds. It can contain other WorkFlows and WorkItems.
protected:
    void applyAction(const Event& event, WorkFlowComponent* source) override;
public:
    WorkFlow(std::string n);                                 // Constructor for a WorkFlow. Takes a string as a parameter to assign as its name.
    ~WorkFlow();                                             // Destructor for a Workflow, recursively deleting the children of a composite when called.
    bool addChild(WorkFlowComponent *c);                     // Adds a child of type WorkFlowComponent to the children vector of the WorkFlow. Returns true if added and false if the child is already added.
    WorkFlowComponent *removeChild(WorkFlowComponent *c);    // Removes a child of type WorkFlowComponent to the children vector of the WorkFlow and returns it if found, otherwise it returns a nullptr.
    WorkFlowMemento *createMemento() override;               // Creates a memento of the WorkFlow and returns it, storing all current children and the state the workflow is currently in.
    void setMemento(WorkFlowMemento *m) override;            // Sets a memento for the WorkFlow, setting current state and children to what was stored in the memento that is passed as a parameter.
    Iterator *createIterator();                              // Creates an iterator for the WorkFlow so that its children can be iterated through.
    std::vector<WorkFlowComponent *> getChildren() override; // Returns all children of a WorkFlow, traversing the tree-structure recursively.
    std::string getName() override;                          // Returns the name of the WorkFlow.
    void accept(WorkFlowVisitor *v) override;                // Accepts a visitor to work on the WorkFlow using double dispatch.
};

#endif
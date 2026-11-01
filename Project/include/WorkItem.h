#ifndef WORKITEM_H
#define WORKITEM_H
#include "WorkFlowComponent.h"


class Participant;
class WorkFlowMemento;
class WorkFlowVisitor;

class WorkItem : public WorkFlowComponent // The WorkItem class that publically inherits from WorkFLowComponent and WorkFlowObserver, implementing the interfaces and acting as the Leave of the WorkFlow Composite.
{
private:
    Participant *assignedParticipant; // The current Participant assigned to the WorkItem.
    std::string name;                 // The name of the WorkItem.
protected:
    void applyAction(const Event& event, WorkFlowComponent* source) override;
public:
    WorkItem(std::string n);                                 // Constructor for a WorkItem. Takes a string as a parameter to assign as its name.
    ~WorkItem();                                             // Destructor for a WorkItem.
    WorkFlowMemento *createMemento() override;               // Creates a memento of the WorkItem and returns it, storing the current Participant and the state the WorkItem is currently in.
    void setMemento(WorkFlowMemento *m) override;            // Sets a memento for the WorkItem, setting current state and participant to what was stored in the memento that is passed as a parameter.
    Participant *getParticipant();                           // Returns the Participant currently assigned to the WorkItem.
    void setParticipant(Participant *p);                     // Sets teh WorkItem's Participant to the passed in parameter.
    std::vector<WorkFlowComponent *> getChildren() override; // Returns a vector of just the WorkItem.
    std::string getName() override;                          // Returns the name of the WorkItem.
    void accept(WorkFlowVisitor *v) override;                // Accepts a visitor to work on the WorkItem using double dispatch.

  
};

#endif
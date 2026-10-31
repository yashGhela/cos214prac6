#ifndef WORKITEM_H
#define WORKITEM_H
#include "WorkFlowComponent.h"
#include "WorkFlowObserver.h"
class WorkItem : public WorkFlowComponent, public WorkFlowObserver // The WorkItem class that publically inherits from WorkFLowComponent and WorkFlowObserver, implementing the interfaces and acting as the Leave of the WorkFlow Composite.
{
private:
    WorkItemState *state;             // The current state of the WorkItem.
    Participant *assignedParticipant; // The current Participant assigned to the WorkItem.
    std::string name;                 // The name of the WorkItem.

public:
    WorkItem(std::string n);                                 // Constructor for a WorkItem. Takes a string as a parameter to assign as its name.
    ~WorkItem();                                             // Destructor for a WorkItem.
    WorkFlowMemento *createMemento() override;               // Creates a memento of the WorkItem and returns it, storing the current Participant and the state the WorkItem is currently in.
    void setMemento(WorkFlowMemento *m) override;            // Sets a memento for the WorkItem, setting current state and participant to what was stored in the memento that is passed as a parameter.
    void setState(WorkItemState *s) override;                // Sets current state of the WorkItem to the state that is passed as a parameter to the function.
    WorkItemState *getState() override;                      // Returns the current state of the WorkItem.
    Participant *getParticipant();                           // Returns the Participant currently assigned to the WorkItem.
    void setParticipant(Participant *p);                     // Sets teh WorkItem's Participant to the passed in parameter.
    std::vector<WorkFlowComponent *> getChildren() override; // Returns a vector of just the WorkItem.
    void update(Event event, WorkFlowComponent *source);     // Updates the Observers that are observing the WorkItem.
    std::string getName() override;                          // Returns the name of the WorkItem.
    void review(WorkItemState *s) override;                  // A Function using double dispatch to attempt to set the state to InReview of the WorkItem. It will return a simple cout message if the state is not currently set to InProgress.
    void approve(WorkItemState *s) override;                 // A Function using double dispatch to attempt to set the state to Approved or back to InProgress of the WorkItem, through a choice of the reviewer. It will return a simple cout message if the state is not currently set to InReview.
    void cancel(WorkItemState *s) override;                  // A Function using double dispatch to set the state to Cancelled of the WorkItem, using a confirmation by the user.
    void finish(WorkItemState *s) override;                  // A Function using double dispatch to attempt to set the state to Finished of the WorkItem. It will return a simple cout message if the state of the WorkItem is not currently set to Approved.
    void beginWork(WorkItemState *s) override;               // A Function using double dispatch to attempt to set the state to InProgress of the WorkItem. It will return a simple cout message if the state is not currently set to Created.
};

#endif
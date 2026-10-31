#ifndef WORKFLOWCOMPONENT_H
#define WORKFLOWCOMPONENT_H

#include <vector>
#include <string>
#include "ActionObserver.h"
#include "EventSubject.h"

class WorkFlowVisitor;
class WorkFlowMemento;
class WorkFlowComponent: public EventSubject, public ActionObserver // The Abstract Interface of the WorkFlow Composite Structure.
{
private:
protected:
    bool inProgress; // True while the component is being worked on.
    bool inReview;   // True while the component is waiting on a reviewer.
    bool approved;   // True once a reviewer has approved the component.
    bool finished;   // True once the component has been finished.
    bool cancelled;  // True once the component has been cancelled.
                     // if all 5 of the flags are false then the Component has simply been created.
    void applyAction(const Event& event, WorkFlowComponent* source) override;
public:
    WorkFlowComponent() : inProgress(false), inReview(false), approved(false), finished(false), cancelled(false) {} // Constructor with Inline definition, setting all state booleans to false.
    virtual ~WorkFlowComponent();                                                                                   // Virtual Destructor
    virtual WorkFlowMemento *createMemento() = 0;                                                                   // Pure virtual function to create a memento of the WorkFlowComponent.
    virtual void setMemento(WorkFlowMemento *memento) = 0;                                                          // Pure virtual function to set a memento of the WorkFlowComponent.
    virtual std::vector<WorkFlowComponent *> getChildren() = 0;                                                     // Pure virtual function to get the children stored in the WorkFlowComponent. Gets overriden differently for a leaf and a composite.
    virtual std::string getName() = 0;                                                                              // Pure virtual function to get the name of the WorkFlowComponent.
    virtual void accept(WorkFlowVisitor *v) = 0;                                                                    // accepts the vistor and calls v->visit(this) to enable double dispatch so WorkItems and WorkFlows can both be vvisited and have different functionality of the visitor.
    bool isCreated() const { return !inProgress && !inReview && !approved && !finished && !cancelled; }             // evaluates to true only if all state flags are false.
    bool isInProgress() const { return inProgress; }                                                                // Returns the InProgress flag
    bool isInReview() const { return inReview; }                                                                    // Returns the InReview flag
    bool isApproved() const { return approved; }                                                                    // Returns the Approved flag
    bool isFinished() const { return finished; }                                                                    // Returns the finished flag
    bool isCancelled() const { return cancelled; }                                                                  // Returns the Canceld flag
    void setStatus(bool progress, bool review, bool approve, bool finish, bool cancel)                              // Sets the state flags all in one go
    {
        inProgress = progress;
        inReview = review;
        approved = approve;
        finished = finish;
        cancelled = cancel;
    }

};

#endif
/**
 * @file WorkFlowComponent.h
 * @brief Component of the WorkFlow Composite structure.
 */
#ifndef WORKFLOWCOMPONENT_H
#define WORKFLOWCOMPONENT_H

#include <vector>
#include <string>
#include "ActionObserver.h"
#include "EventSubject.h"

class WorkFlowVisitor;
class WorkFlowMemento;

/**
 * @brief Abstract interface of the WorkFlow Composite structure.
 *
 * Plays several roles at once:
 * - Component in the Composite pattern (WorkFlow is the composite, WorkItem the leaf).
 * - Subject in the Observer pattern (through EventSubject).
 * - Observer in the Observer pattern (through ActionObserver), reacting to events by running the matching visitor.
 * - Originator in the Memento pattern (createMemento() / setMemento()).
 *
 * A component's state is held in five boolean flags. If all five are false the
 * component has simply been created.
 */
class WorkFlowComponent: public EventSubject, public ActionObserver // The Abstract Interface of the WorkFlow Composite Structure.
{
private:
protected:
    bool inProgress; ///< True while the component is being worked on.
    bool inReview;   ///< True while the component is waiting on a reviewer.
    bool approved;   ///< True once a reviewer has approved the component.
    bool finished;   ///< True once the component has been finished.
    bool cancelled;  ///< True once the component has been cancelled.
    
    /**
     * @brief Applies the action carried by an event by visiting this component.
     *
     * Looks up the visitor for event.getAction() with visitorFor() and accepts it.
     *
     * @param event The event that fired.
     * @param source The component that raised the event (unused).
     */
    void applyAction(const Event& event, WorkFlowComponent* source) override;
public:
    /** @brief Constructor; sets every state flag to false (the created state). */
    WorkFlowComponent() : inProgress(false), inReview(false), approved(false), finished(false), cancelled(false) {}

    /** @brief Virtual destructor. */
    virtual ~WorkFlowComponent(){};

    /**
     * @brief Creates a memento holding the current state of the component.
     * @return A new memento; the caller takes ownership.
     */
    virtual WorkFlowMemento *createMemento() = 0;

    /**
     * @brief Restores the component's state from a memento.
     * @param memento Memento to restore from.
     */
    virtual void setMemento(WorkFlowMemento *memento) = 0;

    /**
     * @brief Returns the children stored in the component.
     *
     * Overridden differently for a leaf (empty) and a composite.
     */
    virtual std::vector<WorkFlowComponent *> getChildren() = 0;

    /** @brief Returns the name of the component. */
    virtual std::string getName() = 0;

    /**
     * @brief Accepts a visitor and calls v->visit(this) for double dispatch.
     *
     * This lets WorkItems and WorkFlows both be visited while the visitor
     * behaves differently for each.
     *
     * @param v The visitor to accept.
     */
    virtual void accept(WorkFlowVisitor *v) = 0;

    /** @brief True only if all state flags are false (the component was just created). */
    bool isCreated() const { return !inProgress && !inReview && !approved && !finished && !cancelled; }

    /** @brief Returns the in-progress flag. */
    bool isInProgress() const { return inProgress; }

    /** @brief Returns the in-review flag. */
    bool isInReview() const { return inReview; }

    /** @brief Returns the approved flag. */
    bool isApproved() const { return approved; }

    /** @brief Returns the finished flag. */
    bool isFinished() const { return finished; }

    /** @brief Returns the cancelled flag. */
    bool isCancelled() const { return cancelled; }

    /**
     * @brief Sets all state flags in one go.
     * @param progress New in-progress flag.
     * @param review New in-review flag.
     * @param approve New approved flag.
     * @param finish New finished flag.
     * @param cancel New cancelled flag.
     */
    void setStatus(bool progress, bool review, bool approve, bool finish, bool cancel)
    {
        inProgress = progress;
        inReview = review;
        approved = approve;
        finished = finish;
        cancelled = cancel;
    }

};

#endif
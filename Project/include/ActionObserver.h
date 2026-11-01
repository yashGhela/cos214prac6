/**
 * @file ActionObserver.h
 * @brief Template-method observer that applies an Event's action.
 */
#ifndef ACTIONOBSERVER_H
#define ACTIONOBSERVER_H

#include "WorkFlowObserver.h"

/**
 * @brief Observer that takes the role of the normal observer so incoming actions are handled uniformly.
 *
 * update() is a Template Method: it checks shouldReact(), then calls
 * applyAction(), then afterUpdate(). Subclasses customise those steps. A
 * general WorkFlowObserver is kept as the base so the design can still be
 * expanded with other kinds of observer.
 */

class ActionObserver : public WorkFlowObserver {
public:
    /** @brief Destructor. */
    ~ActionObserver(){};

    /**
     * @brief Handles a notification (final template method).
     *
     * Does nothing if shouldReact() returns false. Otherwise calls
     * applyAction() followed by afterUpdate().
     *
     * @param event The event that fired.
     * @param source The component that raised the event (may be nullptr).
     */
    void update(const Event& event, WorkFlowComponent* source) final {
        if (!shouldReact(event, source)) return;
        applyAction(event, source);
        afterUpdate(event);
    }

protected:
    /**
    * @brief Performs the action carried by the event (mandatory step).
    * @param event The event that fired.
    * @param source The component that raised the event.
    */
    virtual void applyAction(const Event& event, WorkFlowComponent* source) = 0;

    /**
    * @brief Hook deciding whether this observer reacts to the event.
    * @return true to react (the default), false to ignore the event.
    */
    virtual bool shouldReact(const Event&, WorkFlowComponent*) const { return true; }
    
    /** @brief Hook run after applyAction(); does nothing by default. */
    virtual void afterUpdate(const Event&) {}
};

#endif
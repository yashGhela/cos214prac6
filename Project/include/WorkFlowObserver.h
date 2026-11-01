/**
 * @file WorkFlowObserver.h
 * @brief Abstract Observer interface of the Observer pattern.
 */
#ifndef WORKFLOWOBSERVER_H
#define WORKFLOWOBSERVER_H

#include "Event.h"

class WorkFlowComponent;

/**
 * @brief Abstract observer that is told when an Event it is attached to fires.
 *
 * Observer role in the Observer pattern. Concrete observers (such as
 * ActionObserver and its subclasses) implement update().
 */
class WorkFlowObserver
{
private:
public:
    /** @brief Destructor. */
    inline ~WorkFlowObserver() {};
    
    /**
     * @brief Called by an EventSubject when an event this observer is attached to fires.
     * @param event The event that fired.
     * @param source The component that raised the event (may be nullptr).
     */
    virtual void update(const Event &event, WorkFlowComponent *source) = 0;
};

#endif

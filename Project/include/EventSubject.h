/**
 * @file EventSubject.h
 * @brief Subject role of the Observer pattern.
 */
#ifndef EVENTSUBJECT_H
#define EVENTSUBJECT_H

#include "WorkFlowObserver.h"
#include "Event.h"

class WorkFlowComponent;

/**
 * @brief Subject that attaches observers to events and notifies them.
 *
 * The observer list lives in each Event, so one subject can serve many
 * events. WorkFlowComponent inherits from this class.
 */
class EventSubject {
public:
    /** @brief Virtual destructor. */
    virtual ~EventSubject();

    /**
     * @brief Attaches an observer to an event.
     *
     * Attaching an observer that is already attached has no effect.
     *
     * @param event Event to listen to.
     * @param observer Observer to attach (not owned).
     */
    virtual void Attach(Event& event, WorkFlowObserver* observer);
    
    /**
     * @brief Detaches an observer from an event.
     * @param event Event to stop listening to.
     * @param observer Observer to detach.
     */
    virtual void Detach(Event& event, WorkFlowObserver* observer);
    
    /**
     * @brief Notifies every observer attached to an event.
     *
     * Works on a copy of the observer list, so an observer may detach
     * itself while it is being notified.
     *
     * @param event The event that fired.
     * @param source The component that raised the event (may be nullptr).
     */
    virtual void notify(const Event& event, WorkFlowComponent* source);
};

#endif
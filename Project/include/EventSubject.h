#ifndef EVENTSUBJECT_H
#define EVENTSUBJECT_H

#include "WorkFlowObserver.h"
#include "Event.h"

class WorkFlowComponent;

class EventSubject {
public:
    virtual ~EventSubject();

    virtual void Attach(Event& event, WorkFlowObserver* observer);
    virtual void Detach(Event& event, WorkFlowObserver* observer);
    virtual void notify(const Event& event, WorkFlowComponent* source);
};

#endif
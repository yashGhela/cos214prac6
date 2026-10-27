#ifndef EVENTSUBJECT_H
#define EVENTSUBJECT_H

#include "WorkFlowObserver.h"
#include "Event.h"

#include <algorithm>
#include <vector>

using namespace std;
class WorkflowComponent;

class EventSubject{
private:
    vector<WorkFlowObserver*> observerList;
public:
    virtual void Attach(WorkFlowObserver* observer);
    virtual void Detach(WorkFlowObserver* observer);
    virtual void notify(Event event,WorkflowComponent* source);
    virtual ~EventSubject();
};

#endif


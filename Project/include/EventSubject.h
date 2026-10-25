#ifndef EVENTSUBJECT_H
#define EVENTSUBJECT_H

#include "WorkFlowObserver.h"

#include <algorithm>
#include <vector>

using namespace std;

class EventSubject{
private:
    vector<WorkFlowObserver*> observerList;
public:
    virtual void Attach(WorkFlowObserver* observer)=0;
    virtual void Detach(WorkFlowObserver* observer)=0;
    virtual void notify()=0;
    virtual ~EventSubject();

};

#endif


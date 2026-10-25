#ifndef WORKFLOWOBSERVER_H
#define WORKFLOWOBSERVER_H

#include "Event.h"

class WorkFlowComponent;

class WorkFlowObserver{
private:
    
public:
    virtual ~WorkFlowObserver() = 0;
	virtual void update(Event event,WorkflowComponent* source) = 0;

};

#endif


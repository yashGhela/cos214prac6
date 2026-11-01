#ifndef WORKFLOWOBSERVER_H
#define WORKFLOWOBSERVER_H

#include "Event.h"

class WorkFlowComponent;

class WorkFlowObserver
{
private:
public:
    inline ~WorkFlowObserver() {};
    virtual void update(const Event &event, WorkFlowComponent *source) = 0;
};

#endif

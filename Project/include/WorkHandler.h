#pragma once 

#include "WorkFlowComponent.h"
#include "WorkItem.h"
#include "Event.h"


class WorkFlowComponent;
class WorkItem;



class WorkHandler{

    private:
    WorkHandler* successor;

    public:
    virtual ~WorkHandler();
    virtual void update(const Event& event, WorkFlowComponent* source) const =0;
    virtual void setSuccessor(WorkHandler* next)=0;
    virtual void handleRequest(WorkItem w)=0;
    virtual void process(){};

};
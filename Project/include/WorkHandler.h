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
    virtual void setSuccessor(WorkHandler* next){successor=next;};
    virtual void handleRequest(WorkItem& item)=0;
    virtual void process(WorkItem& item)=0;

};
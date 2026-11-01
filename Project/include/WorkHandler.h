#pragma once 


#include "Event.h"


class WorkFlowComponent;
class WorkItem;



class WorkHandler{

    protected:
    WorkHandler* successor;

    public:
    virtual ~WorkHandler();
    virtual void update(const Event& event, WorkFlowComponent* source) const =0;
    virtual void setSuccessor(WorkHandler* next){successor=next;};
    virtual bool handleRequest(WorkItem* item)=0;
    virtual void process(WorkItem& item)=0;

};



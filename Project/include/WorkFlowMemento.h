#ifndef WORKFLOWMEMENTO_H
#define WORKFLOWMEMENTO_H
#include "map"
#include "string"

class WorkItemState;

class WorkFlowMemento{

    friend class WorkFlow;
    friend class WorkItem;

private:
    std::map<std::string, WorkItemState*> getState();
    void setState(std::map<std::string, WorkItemState*> state);
    std::map<std::string, WorkItemState*> state;



};

#endif
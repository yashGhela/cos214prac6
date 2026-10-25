#ifndef WORKFLOWMEMENTO_H
#define WORKFLOWMEMENTO_H
#include "map"
#include "string"

class WorkItemState;

class WorkFlowMemento{

public:
    std::map<std::string, WorkItemState*> getState();
    void setState(std::map<std::string, WorkItemState*> state);

private:
    std::map<std::string, WorkItemState*> state;

};

#endif
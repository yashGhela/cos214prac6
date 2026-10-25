#ifndef WORKFLOWMEMENTO_H
#define WORKFLOWMEMENTO_H
#include "WorkItemState.h"
#include "map"
#include "string"

class WorkFlowMemento{

public:
    std::map<std::string, WorkItemState*> getState();
    void setState(std::map<std::string, WorkItemState*> state);

private:
    std::map<std::string, WorkItemState*> state;

};

#endif
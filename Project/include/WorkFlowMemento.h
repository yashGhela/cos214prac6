#ifndef WORKFLOWMEMENTO_H
#define WORKFLOWMEMENTO_H
#include "WorkFlowState.h"

class WorkFlowMemento{

public:
    WorkFlowState getState();
    void setState(WorkFlowState state);

private:
    map<WorkItemState*> state;

};

#endif
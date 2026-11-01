#ifndef WORKFLOWMEMENTO_H
#define WORKFLOWMEMENTO_H
#include "map"
#include "string"
#include <vector>

class WorkItemState;

class WorkFlowMemento
{

    friend class WorkFlow;
    friend class WorkItem;

private:
    std::map<std::string, std::vector<bool>> getState();
    void setState(std::map<std::string, std::vector<bool>> state);
    std::map<std::string, std::vector<bool>> state;
};

#endif
#include "WorkFlowMemento.h"

std::map<std::string, WorkItemState *> WorkFlowMemento::getState()
{
    return state;
}

void WorkFlowMemento::setState(std::map<std::string, WorkItemState *> state)
{
    this->state = state;
}

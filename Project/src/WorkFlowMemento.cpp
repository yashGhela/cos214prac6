/**
 * @file WorkFlowMemento.cpp
 * @brief Implementation of WorkFlowMemento.
 */
#include "WorkFlowMemento.h"

std::map<std::string, std::vector<bool>> WorkFlowMemento::getState()
{
    return state;
}

void WorkFlowMemento::setState(std::map<std::string, std::vector<bool>> state)
{
    this->state = state;
}

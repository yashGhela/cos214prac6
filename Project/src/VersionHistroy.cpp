/**
 * @file VersionHistroy.cpp
 * @brief Implementation of VersionHistory.
 */
#include "VersionHistory.h"
#include "VersionIterator.h"

VersionHistory::~VersionHistory()
{

    for(std::map<int, WorkFlowMemento*>::iterator it = mementos.begin(); it != mementos.end(); ++it)
    {
        delete it->second;
    }
    mementos.clear();

}

Iterator *VersionHistory::createIterator()
{
    return new VersionIterator(&mementos);
}

void VersionHistory::addMemento(int version, WorkFlowMemento *memento)
{
    mementos[version] = memento;
}

WorkFlowMemento* VersionHistory::getMemento(int version)
{
    std::map<int, WorkFlowMemento*>::iterator it = mementos.find(version);
    return it == mementos.end() ? nullptr : it->second;
}

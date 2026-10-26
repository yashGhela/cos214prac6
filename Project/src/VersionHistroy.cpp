#include "VersionHistory.h"
#include "VersionIterator.h"

VersionHistory::~VersionHistory()
{

    for(std::map<int, WorkFlowMemento*>::iterator it = mementos.begin(); it != memetos.end(); ++it)
    {
        delete it->second;
    }
    mementos.clear();

}

Iterator *VersionHistory::createIterator()
{
    return new VersionIterator(mementos);
}

void VersionHistory::addMemento(int version, WorkFlowMemento *memento)
{
    mementos[version] = memnto;
}

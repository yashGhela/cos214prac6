#ifndef VERSIONHISTORY_H
#define VERSIONHISTORY_H
#include "WorkFlowMemento.h"
#include "Aggregate.h"
#include "map"

class VersionHistory : public Aggregate{

public:
    ~VersionHistory();
    Iterator* createIterator() override;
    void addMemento(int version, WorkFlowMemento* memento);

private:
    std::map<int, WorkFlowMemento*> mementos;

};

#endif
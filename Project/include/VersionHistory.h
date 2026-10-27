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
    WorkFlowMemento *getMemento(int version);

private:
    std::map<int, WorkFlowMemento*> mementos;

};

#endif
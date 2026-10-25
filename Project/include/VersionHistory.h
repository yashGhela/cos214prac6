#ifndef VERSIONHISTORY_H
#define VERSIONHISTORY_H
#include "WorkFlowMemento.h"
#include "Aggregate.h"
#include "map"

class VersionHistory : public Aggregate{

public:
    Iterator* createIterator() override;

private:
    std::map<int, WorkFlowMemento*> mementos;

};

#endif
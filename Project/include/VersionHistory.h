#ifndef VERSIONHISTORY_H
#define VERSIONHISTORY_H
#include "WorkFlowMemento.h"
#include "Iterator.h"

class VersionHistory : public Aggregate{

public:
    Iterator* createIterator();

private:
    map<WorkFlowMemento> mementos;

};

#endif
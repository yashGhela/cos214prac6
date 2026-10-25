#ifndef VERSIONITERATOR_H
#define VERSIONITERATOR_H

#include "Iterator.h"
#include "WorkFlowMemento.h"
#include <map>

class VersionIterator:public Iterator{
    private:
        std::map<int, WorkFlowMemento*>* versions;
        std::map<int, WorkFlowMemento*>::iterator current;       
    public:
        VersionIterator(std::map<int, Memento*>* versions);
        void next();
        void first();
        bool isDone();
        WorkFlowMemento* currentItem();
};

#endif
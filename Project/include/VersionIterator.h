#ifndef VERSIONITERATOR_H
#define VERSIONITERATOR_H

#include "Iterator.h"
#include "VersionHistory.h"
#include <map>

class VersionIterator:public Iterator{
    private:
        std::map<int, VersionHistory*>* versions;
        std::map<int, VersionHistory*>::iterator current;       
    public:
        VersionIterator(std::map<int, VersionHistory*>* versions);
        void next();
        void first();
        bool isDone();
        VersionHistory* currentItem();
};

#endif
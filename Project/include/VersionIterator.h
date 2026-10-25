#ifndef VERSIONITERATOR_H
#define VERSIONITERATOR_H

#include "Iterator.h"

class VersionIterator:public Iterator{
    private:
    public:
        void next();
        void first();
        bool isDone();
        void currentItem();
};

#endif
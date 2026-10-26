#ifndef EVENTSUBJECT_H
#define EVENTSUBJECT_H

#include "Iterator.h"

class WorkFlowIterator:public Iterator{
    private:
    public:
        void next();
        void first();
        bool isDone();
        void currentItem();
};

#endif
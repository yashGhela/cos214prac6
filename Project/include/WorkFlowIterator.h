#ifndef WORKFLOWITERATOR_H
#define WORKFLOWITERATOR_H

#include "Iterator.h"
#include "WorkFlow.h"
#include <map>
#include <vector>

class WorkFlowIterator:public Iterator{
    private:
        std::vector<WorkFlowComponent*> items;
        int position;

        void getTheChilds(WorkFlow* wf);
    public:
        WorkFlowIterator(WorkFlow* wf);
        void next();
        void first();
        bool isDone();
        WorkFlowComponent* currentItem();
};

#endif
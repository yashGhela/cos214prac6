#pragma once

#include "ProcessHandler.h"
#include "WorkItem.h"

class WorkItem;

template <typename T>
class ProcessHandlerTemplate: public ProcessHandler{
    public:
    void process(WorkItem& item) override;

};
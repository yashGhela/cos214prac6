#pragma once

#include "ProcessHandler.h"

template <typename T>
class ProcessHandlerTemplate: public ProcessHandler{
    public:
    void process(WorkItem& item) override;

};
#pragma once

#include "WorkFlowComponent.h"
#include "WorkHandler.h"
#include "WorkItem.h"
#include "Event.h"
#include "ProcessHandler.h"

class Event;
class WorkFlowComponent;
class WorkItem;

template <typename T>
class HandlerTemplate: public WorkHandler{

    private:
    ProcessHandler* processHandler;
    public:

    explicit HandlerTemplate(ProcessHandler *ph = nullptr) : processHandler(ph) {}

    void update(const Event& event, WorkFlowComponent* source) override 

    void process() override{
        if (processHandler) processHandler->process();
    }


};
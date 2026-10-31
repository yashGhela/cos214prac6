#pragma once

#include "WorkFlowComponent.h"
#include "WorkHandler.h"
#include "WorkItem.h"
#include "Event.h"
#include "ProcessHandler.h"


class WorkFlowComponent;
class WorkItem;
class ActionObserver;

template <typename T>
class HandlerTemplate: public WorkHandler, public ActionObserver{

    private:
    ProcessHandler* processHandler;
    public:

    HandlerTemplate(ProcessHandler *ph = nullptr) : processHandler(ph) {}

    
    bool handleRequest(WorkItem& item) override;
    void process(WorkItem& item) override{
        if (processHandler) processHandler->process(item);
    }

    protected:
    void applyAction(const Event& event, WorkFlowComponent* source) override;


};


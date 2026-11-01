/**
 * @file HandlerTemplate.h
 * @brief Templated concrete handler for the Chain of Responsibility.
 */
#pragma once

#include "WorkFlowComponent.h"
#include "WorkHandler.h"
#include "WorkItem.h"
#include "Event.h"
#include "ProcessHandler.h"


class WorkFlowComponent;
class WorkItem;
class ActionObserver;

/**
 * @brief Concrete handler in the chain that is also an observer.
 *
 * Acts as a WorkHandler (Chain of Responsibility) and as an ActionObserver
 * (Observer). The actual processing is delegated to an optional ProcessHandler.
 *
 * @tparam T Type used to specialise the handler.
 */
template <typename T>
class HandlerTemplate: public WorkHandler, public ActionObserver{

    private:
    ProcessHandler* processHandler;
    public:

    /**
     * @brief Constructor.
     * @param ph The process handler to delegate to (not owned, may be nullptr).
     */
    HandlerTemplate(ProcessHandler *ph = nullptr) : processHandler(ph) {}

    /**
     * @brief Handles a work item, or passes it to the successor.
     * @param item The work item to handle.
     * @return true if the item was handled, false if no handler in the chain could.
     */
    bool handleRequest(WorkItem* item) override;
    
    /**
     * @brief Processes a work item by delegating to the process handler, if there is one.
     * @param item The work item to process.
     */
    void process(WorkItem& item) override{
        if (processHandler) processHandler->process(item);
    }

    // protected:
    // void applyAction(const Event& event, WorkFlowComponent* source) override;


};


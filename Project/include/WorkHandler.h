/**
 * @file WorkHandler.h
 * @brief Abstract Handler of the Chain of Responsibility pattern.
 */
#pragma once 


#include "Event.h"


class WorkFlowComponent;
class WorkItem;


/**
 * @brief Abstract handler that processes a WorkItem or passes it to its successor.
 *
 * Handlers are linked with setSuccessor() to form a chain. A Participant
 * sends its work items to the head of the chain.
 */
class WorkHandler{

    protected:
    WorkHandler* successor; ///< Next handler in the chain (not owned).

    public:
    /** @brief Virtual destructor. */
    virtual ~WorkHandler();
    
    /**
     * @brief Reacts to an event.
     * @param event The event that fired.
     * @param source The component that raised the event.
     */
    virtual void update(const Event& event, WorkFlowComponent* source) const =0;
    
    /**
     * @brief Sets the next handler in the chain.
     * @param next The successor (not owned).
     */
    virtual void setSuccessor(WorkHandler* next){successor=next;};
    
    /**
     * @brief Tries to handle a work item, passing it down the chain if this handler cannot.
     * @param item The work item to handle.
     * @return true if some handler in the chain handled the item.
     */
    virtual bool handleRequest(WorkItem* item)=0;
    
    /**
     * @brief Performs the processing for a work item.
     * @param item The work item to process.
     */
    virtual void process(WorkItem& item)=0;

};



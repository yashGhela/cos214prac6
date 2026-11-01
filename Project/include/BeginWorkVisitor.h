/**
 * @file BeginWorkVisitor.h
 * @brief Visitor that moves a component from created to in-progress.
 */
#ifndef BEGINWORKVISITOR_H
#define BEGINWORKVISITOR_H
#include "WorkFlowVisitor.h"
#include <iostream>

/**
 * @brief Visitor that moves a component from created to in-progress.
 *
 * Only changes the state flags if the component is in the created state; otherwise it prints a message and leaves the component unchanged.
 */
class BeginWorkVisitor : public WorkFlowVisitor
{
private:
    /* data */
public:
    /** @brief Constructor. */
    BeginWorkVisitor(/* args */){};
    
    /** @brief Destructor. */
    ~BeginWorkVisitor(){};
    
    /**
     * @brief Applies the transition to a WorkFlow.
     * @param wf The workflow to visit.
     */
    void visit(WorkFlow *wf) override;
    
    /**
     * @brief Applies the transition to a WorkItem.
     * @param wi The work item to visit.
     */
    void visit(WorkItem *wi) override;
};

#endif
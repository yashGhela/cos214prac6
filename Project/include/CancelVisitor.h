/**
 * @file CancelVisitor.h
 * @brief Visitor that cancels a component from any state except finished or cancelled.
 */
#ifndef CANCELVISITOR_H
#define CANCELVISITOR_H
#include "WorkFlowVisitor.h"
#include <iostream>

/**
 * @brief Visitor that cancels a component from any state except finished or cancelled.
 *
 * Asks the user on standard input to confirm (Y/N) before setting the cancelled state.
 */
class CancelVisitor : public WorkFlowVisitor
{
private:
    /* data */
public:
    /** @brief Constructor. */
    CancelVisitor(/* args */){};

    /** @brief Destructor. */
    ~CancelVisitor(){};

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
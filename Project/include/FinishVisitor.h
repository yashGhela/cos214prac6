/**
 * @file FinishVisitor.h
 * @brief Visitor that moves a component from approved to finished.
 */
#ifndef FINISHVISITOR_H
#define FINISHVISITOR_H
#include "WorkFlowVisitor.h"
#include <iostream>

/**
 * @brief Visitor that moves a component from approved to finished.
 *
 * A WorkItem must be approved. A WorkFlow must be approved and every child must be approved, finished or cancelled. Otherwise a message is printed and nothing changes.
 */
class FinishVisitor : public WorkFlowVisitor
{
private:
    /* data */
public:
    /** @brief Constructor. */
    FinishVisitor(/* args */){};

    /** @brief Destructor. */
    ~FinishVisitor(){};

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
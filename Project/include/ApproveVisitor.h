/**
 * @file ApproveVisitor.h
 * @brief Visitor that approves or rejects a component that is in review.
 */
#ifndef APPROVEVISITOR_H
#define APPROVEVISITOR_H
#include "WorkFlowVisitor.h"
#include <iostream>

/**
 * @brief Visitor that approves or rejects a component that is in review.
 *
 * Only works on a component in the review state. It then asks the user on standard input to approve (A) or reject (R): approving sets the approved state, rejecting sends the component back to in-progress.
 */
class ApproveVisitor : public WorkFlowVisitor // Attempts to change the boolean flags of a workflow component that represent state from inReview to inprogess or approved if is possible in its current state.
{
private:
    /* data */
public:
    /** @brief Constructor. */
    ApproveVisitor(/* args */){};

    /** @brief Destructor. */
    ~ApproveVisitor(){};

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
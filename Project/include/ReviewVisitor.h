/**
 * @file ReviewVisitor.h
 * @brief Visitor that moves a component from in-progress to in-review.
 */
#ifndef REVIEWVISITOR_H
#define REVIEWVISITOR_H
#include "WorkFlowVisitor.h"
#include <iostream>

/**
 * @brief Visitor that moves a component from in-progress to in-review.
 *
 * Only changes the state flags if the component is in progress; otherwise it prints a message and leaves the component unchanged.
 */
class ReviewVisitor : public WorkFlowVisitor // Attempts to change the boolean flags of a workflow component that represent state from inProgress to inReview if is possible in its current state.
{
private:
    /* data */
public:
    /** @brief Constructor. */
    ReviewVisitor(/* args */){};
    
    /** @brief Destructor. */
    ~ReviewVisitor(){};
    
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
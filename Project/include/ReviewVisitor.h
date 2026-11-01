#ifndef REVIEWVISITOR_H
#define REVIEWVISITOR_H
#include "WorkFlowVisitor.h"
#include <iostream>
class ReviewVisitor : public WorkFlowVisitor // Attempts to change the boolean flags of a workflow component that represent state from inProgress to inReview if is possible in its current state.
{
private:
    /* data */
public:
    ReviewVisitor(/* args */){};         // constructor
    ~ReviewVisitor(){};                  // destructor
    void visit(WorkFlow *wf) override; // Implementation of the visit function of WorkFlowVisitor for a workflow
    void visit(WorkItem *wi) override; // Implementation of the visit function of WorkFlowVisitor for a workitem
};

#endif
#ifndef CANCELVISITOR_H
#define CANCELVISITOR_H
#include "WorkFlowVisitor.h"
#include <iostream>
class CancelVisitor : public WorkFlowVisitor // Attempts to change the boolean flags of a workflow component that represent state from any state to cancelled.
{
private:
    /* data */
public:
    CancelVisitor(/* args */);         // constructor
    ~CancelVisitor();                  // destructor
    void visit(WorkFlow *wf) override; // Implementation of the visit function of WorkFlowVisitor for a workflow
    void visit(WorkItem *wi) override; // Implementation of the visit function of WorkFlowVisitor for a workitem
};

#endif
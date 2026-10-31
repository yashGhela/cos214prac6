#ifndef FINISHVISITOR_H
#define FINISHVISITOR_H
#include "WorkFlowVisitor.h"
#include <iostream>
class FinishVisitor : public WorkFlowVisitor // Attempts to change the boolean flags of a workflow component that represent state from approved to finished if is possible in its current state.
{
private:
    /* data */
public:
    FinishVisitor(/* args */){};         // constructor
    ~FinishVisitor(){};                  // destructor
    void visit(WorkFlow *wf) override; // Implementation of the visit function of WorkFlowVisitor for a workflow
    void visit(WorkItem *wi) override; // Implementation of the visit function of WorkFlowVisitor for a workitem
};

#endif
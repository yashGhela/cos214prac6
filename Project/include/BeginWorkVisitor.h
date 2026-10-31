#ifndef BEGINWORKVISITOR_H
#define BEGINWORKVISITOR_H
#include "WorkFlowVisitor.h"
#include <iostream>
class BeginWorkVisitor : public WorkFlowVisitor // Attempts to change the boolean flags of a workflow component that represent state from created to inprogess if is possible in its current state.
{
private:
    /* data */
public:
    BeginWorkVisitor(/* args */){};      // constructor
    ~BeginWorkVisitor(){};               // destructor
    void visit(WorkFlow *wf) override; // Implementation of the visit function of WorkFlowVisitor for a workflow
    void visit(WorkItem *wi) override; // Implementation of the visit function of WorkFlowVisitor for a workitem
};

#endif
#ifndef APPROVEVISITOR_H
#define APPROVEVISITOR_H
#include "WorkFlowVisitor.h"
#include <iostream>
class ApproveVisitor : public WorkFlowVisitor // Attempts to change the boolean flags of a workflow component that represent state from inReview to inprogess or approved if is possible in its current state.
{
private:
    /* data */
public:
    ApproveVisitor(/* args */);        // constructor
    ~ApproveVisitor();                 // destructor
    void visit(WorkFlow *wf) override; // Implementation of the visit function of WorkFlowVisitor for a workflow
    void visit(WorkItem *wi) override; // Implementation of the visit function of WorkFlowVisitor for a workitem
};

#endif
#ifndef WORKFLOWVISITOR_H
#define WORKFLOWVISITOR_H
#include <iostream>
class WorkFlow;
class WorkItem;

class WorkFlowVisitor // Abstract visitor with one operation that gets applied to either a WorkFlow or a WorkItem.
{
private:
    /* data */
public:
    WorkFlowVisitor(/* args */)=default;          // constructor.
    virtual ~WorkFlowVisitor()= default;        // virtual destructor.
    virtual void visit(WorkFlow *wf) = 0; // abstract function to visit a WorkFlow Object.
    virtual void visit(WorkItem *wi) = 0; // abstract function to visit a WorkItem Object.
};

#endif
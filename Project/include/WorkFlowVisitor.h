/**
 * @file WorkFlowVisitor.h
 * @brief Abstract Visitor of the Visitor pattern.
 */
#ifndef WORKFLOWVISITOR_H
#define WORKFLOWVISITOR_H
#include <iostream>
class WorkFlow;
class WorkItem;

/**
 * @brief Abstract visitor with one operation applied to either a WorkFlow or a WorkItem.
 *
 * Concrete visitors implement one state transition each (begin, review,
 * approve, finish, cancel). A component calls its visitor through
 * WorkFlowComponent::accept(), which gives double dispatch.
 */
class WorkFlowVisitor
{
private:
    /* data */
public:
    /** @brief Constructor. */
    WorkFlowVisitor(/* args */)=default;          // constructor.
    
    /** @brief Virtual destructor. */
    virtual ~WorkFlowVisitor()= default;        // virtual destructor.
    
    /**
     * @brief Visits a WorkFlow (composite).
     * @param wf The workflow to visit.
     */
    virtual void visit(WorkFlow *wf) = 0; // abstract function to visit a WorkFlow Object.
    
    /**
     * @brief Visits a WorkItem (leaf).
     * @param wi The work item to visit.
     */
    virtual void visit(WorkItem *wi) = 0; // abstract function to visit a WorkItem Object.
};

#endif
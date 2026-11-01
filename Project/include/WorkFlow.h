/**
 * @file WorkFlow.h
 * @brief Composite class of the WorkFlow Composite structure.
 */
#ifndef WORKFLOW_H
#define WORKFLOW_H
#include "WorkFlowComponent.h"
#include "Aggregate.h"

/**
 * @brief A workflow that groups other workflows and work items.
 *
 * Publicly derived from WorkFlowComponent and Aggregate. It acts as the
 * composite in the WorkFlow Composite structure and as the concrete
 * aggregate that can create an iterator over its children.
 *
 * @note A WorkFlow owns its children and deletes them in its destructor.
 */
class WorkFlow : public WorkFlowComponent, public Aggregate // A class publically derived from WorkFlowComponent and Aggregate. It Implements all functions defined in the respective interfaces. It acts as a composite in the WorkFlow Composite structure.
{
private:
    std::string name;                          // The name of the WorkFlow.
    std::vector<WorkFlowComponent *> children; // The children that the WorkFlow holds. It can contain other WorkFlows and WorkItems.
protected:
    /**
     * @brief Applies the action carried by an event by visiting this workflow.
     * @param event The event that fired.
     * @param source The component that raised the event (unused).
     */
    void applyAction(const Event& event, WorkFlowComponent* source) override;
public:
    /**
     * @brief Constructor.
     * @param n Name of the WorkFlow.
     */
    WorkFlow(std::string n);
    
    /** @brief Destructor; recursively deletes the children. */
    ~WorkFlow();
    
    /**
     * @brief Adds a child to the WorkFlow.
     * @param c The child to add; the WorkFlow takes ownership.
     * @return true if added, false if the child was already added.
     */
    bool addChild(WorkFlowComponent *c);
    
    /**
     * @brief Removes a child from the WorkFlow without deleting it.
     * @param c The child to remove.
     * @return The removed child if found, otherwise nullptr.
     */
    WorkFlowComponent *removeChild(WorkFlowComponent *c);
    
    /**
     * @brief Creates a memento of the WorkFlow.
     *
     * Stores the state flags of the WorkFlow and of all its children.
     *
     * @return A new memento; the caller takes ownership.
     */
    WorkFlowMemento *createMemento() override;
    
    /**
     * @brief Restores the WorkFlow and its children from a memento.
     * @param m Memento to restore from.
     */
    void setMemento(WorkFlowMemento *m) override;
    
    /**
     * @brief Creates an iterator over the WorkFlow's children (including nested ones).
     * @return A new WorkFlowIterator; the caller takes ownership.
     */
    Iterator *createIterator() override;
    
    /** @brief Returns the direct children of the WorkFlow. */
    std::vector<WorkFlowComponent *> getChildren() override;
    
    /** @brief Returns the name of the WorkFlow. */
    std::string getName() override;
    
    /**
     * @brief Accepts a visitor using double dispatch.
     * @param v The visitor to accept.
     */
    void accept(WorkFlowVisitor *v) override;
};

#endif
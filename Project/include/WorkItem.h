/**
 * @file WorkItem.h
 * @brief Leaf class of the WorkFlow Composite structure.
 */
#ifndef WORKITEM_H
#define WORKITEM_H
#include "WorkFlowComponent.h"


class Participant;
class WorkFlowMemento;
class WorkFlowVisitor;

/**
 * @brief A single unit of work; the leaf of the WorkFlow composite.
 *
 * Publicly inherits from WorkFlowComponent (and so is also a subject and an
 * observer) and implements its interface. A WorkItem can be assigned to one
 * Participant.
 */
class WorkItem : public WorkFlowComponent
{
private:
    Participant *assignedParticipant; ///< The current Participant assigned to the WorkItem.
    std::string name;                 ///< The name of the WorkItem.
protected:
    /**
     * @brief Applies the action carried by an event by visiting this work item.
     * @param event The event that fired.
     * @param source The component that raised the event (unused).
     */
    void applyAction(const Event& event, WorkFlowComponent* source) override;
public:
    /**
     * @brief Constructor.
     * @param n Name of the WorkItem.
     */
    WorkItem(std::string n);
    
    /** @brief Destructor. */
    ~WorkItem();
    
    /**
     * @brief Creates a memento of the WorkItem holding its state flags.
     * @return A new memento; the caller takes ownership.
     */
    WorkFlowMemento *createMemento() override;
    
    /**
     * @brief Restores the WorkItem's state flags from a memento.
     * @param m Memento to restore from.
     */
    void setMemento(WorkFlowMemento *m) override;
    
    /** @brief Returns the Participant currently assigned to the WorkItem (may be nullptr). */
    Participant *getParticipant();
    
    /**
     * @brief Assigns a Participant to the WorkItem.
     * @param p The Participant to assign (not owned).
     */
    void setParticipant(Participant *p);
    
    /** @brief Returns an empty vector, since a WorkItem is a leaf. */
    std::vector<WorkFlowComponent *> getChildren() override;
    
    /** @brief Returns the name of the WorkItem. */
    std::string getName() override;
    
    /**
     * @brief Accepts a visitor using double dispatch.
     * @param v The visitor to accept.
     */
    void accept(WorkFlowVisitor *v) override;

  
};

#endif
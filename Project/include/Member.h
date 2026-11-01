/**
 * @file Member.h
 * @brief Leaf of the Participant Composite structure.
 */
#pragma once
#include "Participant.h"

/**
 * @brief A single person who works on one WorkItem at a time.
 */
class Member: public Participant{

    WorkItem* memberWork; ///< The work item this member is currently working on (not owned).

    public:

    /**
     * @brief Constructor.
     * @param name Name of the member.
     */
    Member(std::string name):Participant(name){};
    
    /** @brief Sends the member's current work item to the handler chain. */
    void performWork() override;
    
    /** @brief Does work by calling performWork(). */
    void doWork() override;
    
    /** @brief Claims the first assigned work item that has no participant yet and makes it the current work. */
    void getWork() override;
    
    /** @brief Submits the current work item by visiting it with a FinishVisitor. */
    void submitWork() override;

};
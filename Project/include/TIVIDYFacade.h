/**
 * @file TIVIDYFacade.h
 * @brief Facade of the Facade pattern; the single entry point to the TIVIDY system.
 */
#ifndef TIVIDYFACADE_H
#define TIVIDYFACADE_H

#include <string>
#include <vector>

class WorkFlow;
class WorkFlowComponent;
class WorkItem;
class Participant;
class VersionHistory;
class WorkHandler;
class WorkFlowObserver;
#include "Team.h";

/**
 * @brief Simple interface that hides the visitors, mementos, participants and handler chain behind a few calls.
 *
 * Owns the current workflow and the version history. Participants, the
 * handler chain and the observer are stored but not owned. Every workflow
 * operation prints a message and does nothing if no workflow has been
 * created yet.
 */
class TIVIDYFacade
{
public:

    /** @brief Constructor; starts with no workflow, an empty version history and no handler chain. */
    TIVIDYFacade();
    
    /** @brief Destructor; deletes the workflow and the version history. */
    ~TIVIDYFacade();

    // Workflow creation and management

    /**
     * @brief Creates a new root workflow, deleting any existing one.
     *
     * The version counter is reset to 0. An empty name is rejected with a message.
     *
     * @param name Name of the new workflow; must not be empty.
     */
    void CreateWorkFlow(const std::string& name);
    
    /** @brief Starts the workflow (created to in-progress) using a BeginWorkVisitor. */
    void StartWorkFlow();
    
    /** @brief Sends the workflow to review (in-progress to in-review) using a ReviewVisitor. */
    void ReviewWorkFlow();
    
    /** @brief Rejects the workflow. Not implemented yet; only prints a message. */
    void RejectWorkFlow();
    
    /** @brief Completes the workflow using a FinishVisitor; it must be approved first. */
    void CompleteWorkFlow();
    
    /** @brief Cancels the workflow using a CancelVisitor, after the user confirms. */
    void CancelWorkFlow();

    // Individual work item management

    /**
     * @brief Starts a single work item (created to in-progress) using a BeginWorkVisitor.
     * @param item The work item to start; nullptr prints a message and does nothing.
     */
    void StartWorkItem(WorkItem* item);

    /**
     * @brief Sends a single work item to review using a ReviewVisitor.
     * @param item The work item to review; nullptr prints a message and does nothing.
     */
    void ReviewWorkItem(WorkItem* item);

    /**
     * @brief Completes a single work item using a FinishVisitor; it must be approved first.
     * @param item The work item to complete; nullptr prints a message and does nothing.
     */
    void CompleteWorkItem(WorkItem* item);

    /**
     * @brief Cancels a single work item using a CancelVisitor, after the user confirms.
     * @param item The work item to cancel; nullptr prints a message and does nothing.
     */
    void CancelWorkItem(WorkItem* item);

    /**
     * @brief Adds a work item as a direct child of the root workflow.
     * @param item The work item to add; the workflow takes ownership.
     * @return true if added; false if there is no workflow, the item is nullptr, or it was already added.
     */
    bool AddWorkItem(WorkItem* item);

    // Workflow inspection

    /** @brief Returns the root workflow, or nullptr if none has been created. */
    WorkFlowComponent* GetCurrentWorkflow();

    /**
     * @brief Returns every WorkFlow in the tree, including the root and nested workflows.
     * @return The workflows in depth-first order (parent before children); empty if there is no workflow.
     */
    std::vector<WorkFlow*> getWorkflows();

    /**
     * @brief Returns every WorkItem in the tree, including those in nested workflows.
     * @return The work items in depth-first order; empty if there is no workflow.
     */
    std::vector<WorkItem*> getWorkItems();

    /**
     * @brief Returns the progress of the workflow as a percentage.
     *
     * Progress is the number of finished work items divided by the number of
     * work items that are not cancelled, counting items in nested workflows.
     *
     * @return Percentage from 0 to 100; 0 if there is no workflow, no work items, or every item is cancelled.
     */
    int getProgress();
    

    // Participant management

    /**
     * @brief Registers a participant with the system and gives it the current handler chain.
     *
     * A participant that is already registered is ignored.
     *
     * @param participant The participant to register (not owned); nullptr prints a message and does nothing.
     */
    void AddParticipant(Participant* participant);

    /** @brief Returns the registered participants. */
    std::vector<Participant*> getParticipants();

    /**
     * @brief Assigns a work item to a registered participant.
     * @param participant The participant; must already be registered with AddParticipant().
     * @param item The work item; must belong to the current workflow.
     * @return true if assigned; false if either pointer is nullptr, the participant is not registered, or the item is not in the workflow.
     */
    bool AssignParticipant(Participant* participant, WorkItem* item);

    /**
     * @brief Returns the members of a team.
     * @param team The team to inspect; may be nullptr.
     * @return The team's members, or an empty vector if team is nullptr.
     */
    std::vector<Participant*> getTeamMembers(Team* team);

    /** @brief Returns the registered participants that are Teams. */
    std::vector<Team*> getTeams();
    
    // Handler management

    /**
     * @brief Sets the head of the handler chain and passes it to every registered participant.
     * @param handler The first handler in the chain (not owned).
     */
    void SetHandlerChain(WorkHandler* handler);

    /**
     * @brief Sends a work item to the handler chain.
     * @param item The work item to process.
     * @return true if a handler in the chain handled the item; false if item is nullptr, no chain is configured, or nobody could handle it.
     */
    bool ProcessWorkItem(WorkItem* item);

    // Version management

    /** @brief Returns the number of the current version. */
    int getVersion();
    
    /**
     * @brief Saves the current state of the workflow as a new version.
     *
     * Increments the version number and stores a memento under it. Does
     * nothing if there is no workflow.
     */
    void saveVersion();

    /**
     * @brief Restores the workflow to a previously saved version.
     *
     * Prints a message and changes nothing if there is no workflow, the
     * pointer is nullptr, or the version does not exist.
     *
     * @param version Pointer to the version number to return to.
     */
    void regressVersion(int* version);

private:

    /** @brief Current root workflow (owned). */
    WorkFlow* workflow;

    // Other system components
    VersionHistory* history;    ///< Saved versions of the workflow (owned).
    WorkHandler* handlerChain;  ///< Head of the handler chain (not owned).
    WorkFlowObserver* observer; ///< Observer attached to the workflow (not owned).

    /** @brief Registered participants (not owned). */
    std::vector<Participant*> participants;

    // Helper functions for hierarchy traversal

    /**
     * @brief Recursively collects a component and every WorkFlow below it.
     * @param component Component to start from; nullptr is ignored.
     * @param result Vector the workflows are appended to.
     */
    void collectWorkflows(
        WorkFlowComponent* component,
        std::vector<WorkFlow*>& result
    );

    /**
     * @brief Recursively collects a component and every WorkItem below it.
     * @param component Component to start from; nullptr is ignored.
     * @param result Vector the work items are appended to.
     */
    void collectWorkItems(
        WorkFlowComponent* component,
        std::vector<WorkItem*>& result
    );

    int currentVersion; ///< Number of the current version.
};

#endif
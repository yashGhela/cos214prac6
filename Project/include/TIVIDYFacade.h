/**
 * @file TIVIDYFacade.h
 * @brief Facade of the Facade pattern; the single entry point to the TIVIDY system.
 */
#ifndef TIVIDYFACADE_H
#define TIVIDYFACADE_H
#include "string"
#include "vector"

class WorkFlowComponent;
class Participant;
class VersionHistory;
class WorkHandler;
class WorkFlowObserver;

/**
 * @brief Simple interface that hides the visitors, mementos and participants behind a few calls.
 *
 * Owns the current workflow and the version history. Participants are stored
 * but not owned. Every workflow operation does nothing if no workflow has
 * been created yet.
 */
class TIVIDYFacade{

public:
    /** @brief Constructor; starts with no workflow and an empty version history. */
    TIVIDYFacade();
    
    /** @brief Destructor; deletes the workflow and the version history. */
    ~TIVIDYFacade();

    /** @brief Starts the workflow (created to in-progress) using a BeginWorkVisitor. */
    void StartWorkFlow();
    
    /**
     * @brief Creates a new workflow, deleting any existing one.
     * @param name Name of the new workflow.
     */
    void CreateWorkFlow(const std::string& name);
    
    /** @brief Sends the workflow to review (in-progress to in-review) using a ReviewVisitor. */
    void ReviewWorkFlow();
    
    /** @brief Rejects the workflow. Not implemented yet; currently does nothing. */
    void RejectWorkFlow();
    
    /** @brief Completes the workflow using a FinishVisitor; it must be approved first. */
    void CompleteWorkFlow();
    
    /** @brief Cancels the workflow using a CancelVisitor, after the user confirms. */
    void CancelWorkFlow();

    /**
     * @brief Adds a participant to the system.
     * @param participant The participant to add (not owned); nullptr is ignored.
     */
    void AddParticipant(Participant* participant);

    /**
     * @brief Returns the progress of the workflow as a percentage.
     *
     * Progress is the number of finished children divided by the number of
     * children that are not cancelled.
     *
     * @return Percentage from 0 to 100, or 0 if there is no workflow or it has no active children.
     */
    int getProgress();

    /** @brief Returns the number of the current version. */
    int getVersion();
    
    /** @brief Saves the current state of the workflow as a new version. */
    void saveVersion();
    
    /**
     * @brief Restores the workflow to a previously saved version.
     * @param version The version number to return to; does nothing if it does not exist.
     */
    void regressVersion(int version);

private:
    WorkFlowComponent* workflow; ///< The current workflow (owned).
    VersionHistory* history; ///< Saved versions of the workflow (owned).
    WorkHandler* handlerChain; ///< Head of the handler chain (not owned).
    WorkFlowObserver* observer; ///< Observer attached to the workflow (not owned).
    std::vector<Participant*> participants; ///< Participants added to the system (not owned).
    int currentVersion; ///< Number of the current version.

};

#endif
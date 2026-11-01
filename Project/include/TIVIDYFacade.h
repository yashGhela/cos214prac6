
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

class TIVIDYFacade
{
public:

    TIVIDYFacade();
    
    /** @brief Destructor; deletes the workflow and the version history. */
    ~TIVIDYFacade();

    // Workflow creation and management
    void CreateWorkFlow(const std::string& name);
    void StartWorkFlow();
    void ReviewWorkFlow();
    
    /** @brief Rejects the workflow. Not implemented yet; currently does nothing. */
    void RejectWorkFlow();
    
    /** @brief Completes the workflow using a FinishVisitor; it must be approved first. */
    void CompleteWorkFlow();
    
    /** @brief Cancels the workflow using a CancelVisitor, after the user confirms. */
    void CancelWorkFlow();

    // Individual work item management
    void StartWorkItem(WorkItem* item);
    void ReviewWorkItem(WorkItem* item);
    void CompleteWorkItem(WorkItem* item);
    void CancelWorkItem(WorkItem* item);
    bool AddWorkItem(WorkItem* item);

    // Workflow inspection
    WorkFlowComponent* GetCurrentWorkflow();
    std::vector<WorkFlow*> getWorkflows();
    std::vector<WorkItem*> getWorkItems();
    int getProgress();
    

    // Participant management
    void AddParticipant(Participant* participant);
    std::vector<Participant*> getParticipants();
    bool AssignParticipant(Participant* participant, WorkItem* item);
    std::vector<Participant*> getTeamMembers(Team* team);
    std::vector<Team*> getTeams();
    
    // Handler management
    void SetHandlerChain(WorkHandler* handler);
    bool ProcessWorkItem(WorkItem* item);

    // Version management
    int getVersion();
    
    /** @brief Saves the current state of the workflow as a new version. */
    void saveVersion();
    void regressVersion(int* version);

private:

    // Current root workflow
    WorkFlow* workflow;

    // Other system components
    VersionHistory* history;
    WorkHandler* handlerChain;
    WorkFlowObserver* observer;

    // Registered participants
    std::vector<Participant*> participants;

    // Helper functions for hierarchy traversal
    void collectWorkflows(
        WorkFlowComponent* component,
        std::vector<WorkFlow*>& result
    );

    void collectWorkItems(
        WorkFlowComponent* component,
        std::vector<WorkItem*>& result
    );

    int currentVersion;
};

#endif

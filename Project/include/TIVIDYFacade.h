
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

class TIVIDYFacade
{
public:

    TIVIDYFacade();
    ~TIVIDYFacade();

    // Workflow creation and management
    void CreateWorkFlow(const std::string& name);
    void StartWorkFlow();
    void ReviewWorkFlow();
    void RejectWorkFlow();
    void CompleteWorkFlow();
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

    // Handler management
    void SetHandlerChain(WorkHandler* handler);
    bool ProcessWorkItem(WorkItem* item);

    // Version management
    int getVersion();
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

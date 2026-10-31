#ifndef TIVIDYFACADE_H
#define TIVIDYFACADE_H
#include "string"
#include "vector"

class WorkFlowComponent;
class Participant;
class VersionHistory;
class WorkHandler;
class WorkFlowObserver;

class TIVIDYFacade{

public:
    TIVIDYFacade();
    ~TIVIDYFacade();

    void StartWorkFlow();
    void CreateWorkFlow(const std::string& name);
    void ReviewWorkFlow();
    void RejectWorkFlow();
    void CompleteWorkFlow();
    void CancelWorkFlow();

    void AddParticipant(Participant* participant);

    int getProgress();
    WorkFlowComponent* GetCurrentWorkflow();
    int getVersion();
    void saveVersion();
    void regressVersion(int version);
    std::vector<WorkFlow*> getWorkflows();

private:
    WorkFlowComponent* workflow;
    VersionHistory* history;
    WorkHandler* handlerChain;
    WorkFlowObserver* observer;

    std::vector<Participant*> participants;
    std::vector<WorkFlow*> workflows;

    int currentVersion;

};

#endif
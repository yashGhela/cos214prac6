#include "TIVIDYFacade.h"
#include "WorkFlow.h"
#include "WorkFlowComponent.h"
#include "WorkItemState.h"
#include "FinishedState.h"
#include "Participant.h"
#include "VersionHistory.h"
#include "WorkHandler.h"
#include "WorkFlowObserver.h"

TIVIDYFacade::TIVIDYFacade() : workflow(nullptr), history(new VersionHistory()), handlerChain(nullptr), observer(nullptr), currentVersion(0)
{}

TIVIDYFacade::~TIVIDYFacade()
{
    delete workflow;
    delete history;
}
WorkFlowComponent* TIVIDYFacade::GetCurrentWorkflow()
{
    return workflow;
}

//perchance add a getWorkflows function to return all nested workflows? 
std::vector<WorkFlow*> getWorkflows(){

}
void TIVIDYFacade::StartWorkFlow()
{
    if(workflow)
    {
        std::cout<<"Starting Workflow"<<std::endl;
        workflow->beginWork(workflow->getState());
    }
}

void TIVIDYFacade::CreateWorkFlow(const std::string &name)
{
    std::cout<<"Creating Workflow"<<std::endl;
    delete workflow;
    workflow = new WorkFlow(name);
}

void TIVIDYFacade::ReviewWorkFlow()
{
    if(workflow)
    {
        std::cout<<"Reviewing Workflow"<<std::endl;
        workflow->review(workflow->getState());
    }
}

void TIVIDYFacade::RejectWorkFlow()
{
    
}

void TIVIDYFacade::CompleteWorkFlow()
{
    if(workflow)
    {
        std::cout<<"Completing Workflow"<<std::endl;
        workflow->finish(workflow->getState());
    }
}

void TIVIDYFacade::CancelWorkFlow()
{
    if(workflow)
    {
        std::cout<<"Canceling Workflow"<<std::endl;
        workflow->cancel(workflow->getState());
    }
}

void TIVIDYFacade::AddParticipant(Participant *participant)
{
    if(participant)
    {
        std::cout<<"Adding Particpant"<<std::endl;
        participants.push_back(participant);
    }
}

int TIVIDYFacade::getProgress()
{
    if(!workflow)
        return 0;

    std::vector<WorkFlowComponent*> children = workflow->getChildren();
    if(children.empty())
        return 0;

    int finished = 0;
    for(size_t i = 0; i < children.size(); ++i)
    {
        if(dynamic_cast<FinishedState*>(children[i]->getState()))
        {
            ++finished;
        }
    }

    return static_cast<int>(finished * 100 / children.size());

}

int TIVIDYFacade::getVersion()
{
    return currentVersion;
}

void TIVIDYFacade::saveVersion()
{
    if(!workflow)
        return;

    history->addMemento(++currentVersion, workflow->createMemento());
    std::cout<<"Saving Version"<<std::endl;

}

void TIVIDYFacade::regressVersion(int version)
{
    if(!workflow)
        return;

    WorkFlowMemento* m = history->getMemento(version);

    if(m)
    {
        workflow->setMemento(m);
        currentVersion = version;
        std::cout<<"Regressing Version"<<std::endl;
    }
}

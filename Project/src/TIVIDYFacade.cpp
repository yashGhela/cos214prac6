/**
 * @file TIVIDYFacade.cpp
 * @brief Implementation of TIVIDYFacade.
 */
#include "TIVIDYFacade.h"
#include "WorkFlow.h"
#include "WorkFlowComponent.h"
#include "FinishVisitor.h"
#include "BeginWorkVisitor.h"
#include "ReviewVisitor.h"
#include "CancelVisitor.h"
#include "Participant.h"
#include "VersionHistory.h"
#include "WorkHandler.h"
#include "WorkFlowObserver.h"

TIVIDYFacade::TIVIDYFacade() : workflow(nullptr), history(new VersionHistory()), handlerChain(nullptr), observer(nullptr), currentVersion(0)
{
}

TIVIDYFacade::~TIVIDYFacade()
{
    delete workflow;
    delete history;
}

void TIVIDYFacade::StartWorkFlow()
{
    if (workflow)
    {
        std::cout << "Starting Workflow" << std::endl;
        BeginWorkVisitor beginWork;
        workflow->accept(&beginWork);
    }
}

void TIVIDYFacade::CreateWorkFlow(const std::string &name)
{
    std::cout << "Creating Workflow" << std::endl;
    delete workflow;
    workflow = new WorkFlow(name);
}

void TIVIDYFacade::ReviewWorkFlow()
{
    if (workflow)
    {
        std::cout << "Reviewing Workflow" << std::endl;
        ReviewVisitor review;
        workflow->accept(&review);
    }
}

void TIVIDYFacade::RejectWorkFlow()
{
}

void TIVIDYFacade::CompleteWorkFlow()
{
    if (workflow)
    {
        std::cout << "Completing Workflow" << std::endl;
        FinishVisitor finish;
        workflow->accept(&finish);
    }
}

void TIVIDYFacade::CancelWorkFlow()
{
    if (workflow)
    {
        std::cout << "Canceling Workflow" << std::endl;
        CancelVisitor cancel;
        workflow->accept(&cancel);
    }
}

void TIVIDYFacade::AddParticipant(Participant *participant)
{
    if (participant)
    {
        std::cout << "Adding Particpant" << std::endl;
        participants.push_back(participant);
    }
}

int TIVIDYFacade::getProgress()
{
    if (!workflow)
        return 0;

    std::vector<WorkFlowComponent *> children = workflow->getChildren();
    if (children.empty())
        return 0;
    int finished = 0;
    int active = 0;
    for (size_t i = 0; i < children.size(); ++i)
    {
        if (children[i]->isCancelled())
        {
            continue;
        }
        ++active;
        if (children[i]->isFinished())
        {
            ++finished;
        }
    }
    return static_cast<int>(finished * 100 / active);
}

int TIVIDYFacade::getVersion()
{
    return currentVersion;
}

void TIVIDYFacade::saveVersion()
{
    if (!workflow)
        return;

    history->addMemento(++currentVersion, workflow->createMemento());
    std::cout << "Saving Version" << std::endl;
}

void TIVIDYFacade::regressVersion(int version)
{
    if (!workflow)
        return;

    WorkFlowMemento *m = history->getMemento(version);

    if (m)
    {
        workflow->setMemento(m);
        currentVersion = version;
        std::cout << "Regressing Version" << std::endl;
    }
}

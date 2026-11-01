
#include "TIVIDYFacade.h"

#include "WorkFlow.h"
#include "WorkFlowComponent.h"
#include "WorkItem.h"

#include "FinishVisitor.h"
#include "BeginWorkVisitor.h"
#include "ReviewVisitor.h"
#include "CancelVisitor.h"

#include "Participant.h"
#include "VersionHistory.h"
#include "WorkHandler.h"
#include "WorkFlowObserver.h"

#include <iostream>

// ==========================================
// CONSTRUCTOR AND DESTRUCTOR
// ==========================================

TIVIDYFacade::TIVIDYFacade()
    : workflow(nullptr),
      history(new VersionHistory()),
      handlerChain(nullptr),
      observer(nullptr),
      currentVersion(0)
{
}

TIVIDYFacade::~TIVIDYFacade()
{
    delete workflow;
    delete history;

    // Participants, handlerChain and observer
    // are not deleted because ownership has not
    // been transferred to the Facade.
}

// ==========================================
// WORKFLOW CREATION AND MANAGEMENT
// ==========================================

void TIVIDYFacade::CreateWorkFlow(const std::string& name)
{
    if (name.empty())
    {
        std::cout << "Workflow name cannot be empty.\n";
        return;
    }

    std::cout << "Creating Workflow: " << name << std::endl;

    delete workflow;
    workflow = new WorkFlow(name);

    currentVersion = 0;
}

void TIVIDYFacade::StartWorkFlow()
{
    if (!workflow)
    {
        std::cout << "No workflow has been created.\n";
        return;
    }

    std::cout << "Starting Workflow\n";

    BeginWorkVisitor beginWork;
    workflow->accept(&beginWork);
}

void TIVIDYFacade::ReviewWorkFlow()
{
    if (!workflow)
    {
        std::cout << "No workflow has been created.\n";
        return;
    }

    std::cout << "Reviewing Workflow\n";

    ReviewVisitor review;
    workflow->accept(&review);
}

void TIVIDYFacade::RejectWorkFlow()
{
    if (!workflow)
    {
        std::cout << "No workflow has been created.\n";
        return;
    }

    // Rejection needs a dedicated rejection operation
    // or visitor. It is not implemented by the supplied
    // Visitor interfaces yet.

    std::cout << "Workflow rejection is not implemented yet.\n";
}

void TIVIDYFacade::CompleteWorkFlow()
{
    if (!workflow)
    {
        std::cout << "No workflow has been created.\n";
        return;
    }

    std::cout << "Completing Workflow\n";

    FinishVisitor finish;
    workflow->accept(&finish);
}

void TIVIDYFacade::CancelWorkFlow()
{
    if (!workflow)
    {
        std::cout << "No workflow has been created.\n";
        return;
    }

    std::cout << "Cancelling Workflow\n";

    CancelVisitor cancel;
    workflow->accept(&cancel);
}

// ==========================================
// INDIVIDUAL WORK ITEM MANAGEMENT
// ==========================================

void TIVIDYFacade::StartWorkItem(WorkItem* item)
{
    if (!item)
    {
        std::cout << "Invalid WorkItem.\n";
        return;
    }

    std::cout << "Starting WorkItem: "
              << item->getName() << std::endl;

    BeginWorkVisitor beginWork;
    item->accept(&beginWork);
}

void TIVIDYFacade::ReviewWorkItem(WorkItem* item)
{
    if (!item)
    {
        std::cout << "Invalid WorkItem.\n";
        return;
    }

    std::cout << "Reviewing WorkItem: "
              << item->getName() << std::endl;

    ReviewVisitor review;
    item->accept(&review);
}

void TIVIDYFacade::CompleteWorkItem(WorkItem* item)
{
    if (!item)
    {
        std::cout << "Invalid WorkItem.\n";
        return;
    }

    std::cout << "Completing WorkItem: "
              << item->getName() << std::endl;

    FinishVisitor finish;
    item->accept(&finish);
}

void TIVIDYFacade::CancelWorkItem(WorkItem* item)
{
    if (!item)
    {
        std::cout << "Invalid WorkItem.\n";
        return;
    }

    std::cout << "Cancelling WorkItem: "
              << item->getName() << std::endl;

    CancelVisitor cancel;
    item->accept(&cancel);
}

// ==========================================
// WORKFLOW INSPECTION
// ==========================================

WorkFlowComponent* TIVIDYFacade::GetCurrentWorkflow()
{
    return workflow;
}

// Recursively collect all nested WorkFlows

void TIVIDYFacade::collectWorkflows(
    WorkFlowComponent* component,
    std::vector<WorkFlow*>& result)
{
    if (!component)
        return;

    WorkFlow* current =
        dynamic_cast<WorkFlow*>(component);

    if (current)
    {
        result.push_back(current);
    }

    std::vector<WorkFlowComponent*> children =
        component->getChildren();

    for (size_t i = 0; i < children.size(); ++i)
    {
        collectWorkflows(children[i], result);
    }
}

// Return all WorkFlows

std::vector<WorkFlow*> TIVIDYFacade::getWorkflows()
{
    std::vector<WorkFlow*> result;

    if (workflow)
    {
        collectWorkflows(workflow, result);
    }

    return result;
}

// Recursively collect all WorkItems

void TIVIDYFacade::collectWorkItems(
    WorkFlowComponent* component,
    std::vector<WorkItem*>& result)
{
    if (!component)
        return;

    WorkItem* item =
        dynamic_cast<WorkItem*>(component);

    if (item)
    {
        result.push_back(item);
    }

    std::vector<WorkFlowComponent*> children =
        component->getChildren();

    for (size_t i = 0; i < children.size(); ++i)
    {
        collectWorkItems(children[i], result);
    }
}

// Return all WorkItems

std::vector<WorkItem*> TIVIDYFacade::getWorkItems()
{
    std::vector<WorkItem*> result;

    if (workflow)
    {
        collectWorkItems(workflow, result);
    }

    return result;
}

// ==========================================
// WORKFLOW PROGRESS
// ==========================================

int TIVIDYFacade::getProgress()
{
    std::vector<WorkItem*> items = getWorkItems();

    if (items.empty())
        return 0;

    int finished = 0;
    int active = 0;

    for (size_t i = 0; i < items.size(); ++i)
    {
        if (items[i]->isCancelled())
            continue;

        ++active;

        if (items[i]->isFinished())
        {
            ++finished;
        }
    }

    if (active == 0)
        return 0;

    return (finished * 100) / active;
}

// ==========================================
// PARTICIPANT MANAGEMENT
// ==========================================

void TIVIDYFacade::AddParticipant(Participant* participant)
{
    if (!participant)
    {
        std::cout << "Invalid Participant.\n";
        return;
    }

    // Avoid registering the same participant twice
    for (size_t i = 0; i < participants.size(); ++i)
    {
        if (participants[i] == participant)
        {
            std::cout << "Participant already registered.\n";
            return;
        }
    }

    participants.push_back(participant);

    // Give participant access to the handler chain
    participant->setChain(handlerChain);

    std::cout << participant->getName()
              << " added successfully.\n";
}

std::vector<Participant*> TIVIDYFacade::getParticipants()
{
    return participants;
}

// Assign participant to WorkItem

bool TIVIDYFacade::AssignParticipant(
    Participant* participant,
    WorkItem* item)
{
    if (!participant || !item)
    {
        std::cout << "Invalid Participant or WorkItem.\n";
        return false;
    }

    // Ensure participant belongs to this Facade
    bool registered = false;

    for (size_t i = 0; i < participants.size(); ++i)
    {
        if (participants[i] == participant)
        {
            registered = true;
            break;
        }
    }

    if (!registered)
    {
        std::cout << "Participant is not registered.\n";
        return false;
    }

    // Ensure item belongs to the current workflow
    std::vector<WorkItem*> items = getWorkItems();

    bool found = false;

    for (size_t i = 0; i < items.size(); ++i)
    {
        if (items[i] == item)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        std::cout << "WorkItem does not belong to this workflow.\n";
        return false;
    }

    participant->assignWork(item);

    std::cout << participant->getName()
              << " assigned to "
              << item->getName() << ".\n";

    return true;
}

// ==========================================
// HANDLER MANAGEMENT
// ==========================================

void TIVIDYFacade::SetHandlerChain(WorkHandler* handler)
{
    handlerChain = handler;

    // Update all registered participants
    for (size_t i = 0; i < participants.size(); ++i)
    {
        participants[i]->setChain(handlerChain);
    }

    std::cout << "Handler chain configured.\n";
}

// Process a WorkItem through the Handler Chain

bool TIVIDYFacade::ProcessWorkItem(WorkItem* item)
{
    if (!item)
    {
        std::cout << "Invalid WorkItem.\n";
        return false;
    }

    if (!handlerChain)
    {
        std::cout << "No Handler Chain configured.\n";
        return false;
    }

    return handlerChain->handleRequest(item);
}

// ==========================================
// VERSION MANAGEMENT
// ==========================================

int TIVIDYFacade::getVersion()
{
    return currentVersion;
}

// Save current workflow state

void TIVIDYFacade::saveVersion()
{
    if (!workflow)
    {
        std::cout << "No workflow to save.\n";
        return;
    }

    history->addMemento(
        ++currentVersion,
        workflow->createMemento()
    );

    std::cout << "Workflow version "
              << currentVersion
              << " saved successfully.\n";
}

// Restore previous workflow state

void TIVIDYFacade::regressVersion(int* version)
{
    if (!workflow)
    {
        std::cout << "No workflow to restore.\n";
        return;
    }

    if (!version)
    {
        std::cout << "Invalid version number.\n";
        return;
    }

    WorkFlowMemento* m =
        history->getMemento(*version);

    if (m)
    {
        workflow->setMemento(m);

        currentVersion = *version;

        std::cout << "Restored Workflow Version "
                  << currentVersion << ".\n";
    }
    else
    {
        std::cout << "Requested version does not exist.\n";
    }
}
bool TIVIDYFacade::AddWorkItem(WorkItem* item)
{
    if (workflow == nullptr || item == nullptr)
        return false;

    return workflow->addChild(item);
}

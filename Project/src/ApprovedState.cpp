#include "ApprovedState.h"

ApprovedState::ApprovedState(WorkFlowComponent *c)
{
    this->context = c;
}

ApprovedState::~ApprovedState()
{
}

std::string ApprovedState::stringStatus() const
{
    return "approved";
}

void ApprovedState::review(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " cannot be reviewed after it has been approved" << std::endl;
    return;
}

void ApprovedState::review(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " cannot be reviewed after it has been approved" << std::endl;
    return;
}

void ApprovedState::approve(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " has been approved already" << std::endl;
    return;
}

void ApprovedState::approve(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " has been approved already" << std::endl;
    return;
}

void ApprovedState::cancel(WorkFlow *wf)
{
    std::cout << "Are you sure you wish to cancel WorkFlow " << wf->getName() << "? (Y/N)" << std::endl;
    char response;
    std::cin >> response;
    while (response != 'Y' && response != 'y' && response != 'N' && response != 'n')
    {
        std::cout << "Please enter Y or N" << std::endl;
        std::cin >> response;
    }
    if (response == 'Y' || response == 'y')
    {
        WorkItemState *state = new CancelledState(this->context);
        this->context->setState(state);
        this->context = nullptr;
    }
    else if (response == 'N' || response == 'n')
    {
        return;
    }
}

void ApprovedState::cancel(WorkItem *wi)
{
    std::cout << "Are you sure you wish to cancel WorkItem " << wi->getName() << "? (Y/N)" << std::endl;
    char response;
    std::cin >> response;
    while (response != 'Y' && response != 'y' && response != 'N' && response != 'n')
    {
        std::cout << "Please enter Y or N" << std::endl;
        std::cin >> response;
    }
    if (response == 'Y' || response == 'y')
    {
        WorkItemState *state = new CancelledState(this->context);
        this->context->setState(state);
        this->context = nullptr;
    }
    else if (response == 'N' || response == 'n')
    {
        return;
    }
}

void ApprovedState::finish(WorkFlow *wf)
{
    std::vector<WorkFlowComponent *> children = wf->getChildren();
    bool allKiddiesFinished = true;
    for (auto child : children)
    {
        if (child->getState()->stringStatus() != "approved" && child->getState()->stringStatus() != "cancelled" && child->getState()->stringStatus() != "finished")
        {
            allKiddiesFinished = false;
            break;
        }
    }
    if (allKiddiesFinished)
    {
        std::cout << "WorkFlow " << wf->getName() << "'s Work Items and Work Flows are approved, finished or cancelled. Finishing." << std::endl;
        WorkItemState *state = new FinishedState(this->context);
        this->context->setState(state);
        this->context = nullptr;
    }
    else
    {
        std::cout << "WorkFlow " << wf->getName() << " still has unapproved Work Item(s) or WorkFlow(s). These must be approved, finished or cancelled before finishing" << std::endl;
    }
}

void ApprovedState::finish(WorkItem *wi)
{
    if (wi->getState()->stringStatus() != "approved")
    {
        std::cout << "WorkItem " << wi->getName() << "has been approved. Finishing." << std::endl;
        WorkItemState *state = new FinishedState(this->context);
        this->context->setState(state);
        this->context = nullptr;
    }
    else
    {
        std::cout << "WorkItem " << wi->getName() << " must be approved before finishing" << std::endl;
    }
}

void ApprovedState::beginWork(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << "has been approved already. Start a new WorkFlow to begin new work. " << std::endl;
    return;
}

void ApprovedState::beginWork(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << "has been approved already. Start a new WorkItem to begin new work. " << std::endl;
    return;
}
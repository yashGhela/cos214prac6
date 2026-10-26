#include "CreatedState.h"

CreatedState::CreatedState(WorkFlowComponent *c)
{
    this->context = c;
}

CreatedState::~CreatedState()
{
}

std::string CreatedState::stringStatus() const
{
    return "created";
}

void CreatedState::review(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " needs to have been in progress before reviewing it." << std::endl;
    return;
}

void CreatedState::review(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " needs to have been in progress before reviewing it." << std::endl;
    return;
}

void CreatedState::approve(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " needs to have been in progress and reviewed before approving it." << std::endl;
    return;
}

void CreatedState::approve(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " needs to have been in progress and reviewed before approving it." << std::endl;
    return;
}

void CreatedState::cancel(WorkFlow *wf)
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

void CreatedState::cancel(WorkItem *wi)
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

void CreatedState::finish(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " needs to have been in progress reviewed, and approved before finishing." << std::endl;
    return;
}

void CreatedState::finish(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " needs to have been in progress reviewed, and approved before finishing." << std::endl;
    return;
}

void CreatedState::beginWork(WorkFlow *wf)
{
    WorkItemState *state = new InProgressState(this->context);
    this->context->setState(state);
    this->context = nullptr;
    std::cout << "WorkFlow " << wf->getName() << " has begun its progress." << std::endl;
}

void CreatedState::beginWork(WorkItem *wi)
{
    WorkItemState *state = new InProgressState(this->context);
    this->context->setState(state);
    this->context = nullptr;
    std::cout << "WorkItem " << wi->getName() << " has begun its progress." << std::endl;
}
#include "InProgressState.h"

InProgressState::InProgressState(WorkFlowComponent *c)
{
    this->context = c;
}

InProgressState::~InProgressState()
{
}

std::string InProgressState::stringStatus() const
{
    return "in progress";
}

void InProgressState::review(WorkFlow *wf)
{
    WorkItemState *state = new InReviewState(this->context);
    this->context->setState(state);
    this->context = nullptr;
    std::cout << "WorkFlow " << wf->getName() << " has begun the review process." << std::endl;
}

void InProgressState::review(WorkItem *wi)
{
    WorkItemState *state = new InReviewState(this->context);
    this->context->setState(state);
    this->context = nullptr;
    std::cout << "WorkItem " << wi->getName() << " has begun the review process." << std::endl;
}

void InProgressState::approve(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " needs to be reviewed before being approved." << std::endl;
    return;
}

void InProgressState::approve(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " needs to be reviewed before being approved." << std::endl;
    return;
}

void InProgressState::cancel(WorkFlow *wf)
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

void InProgressState::cancel(WorkItem *wi)
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

void InProgressState::finish(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " needs to be reviewed and approved before finishing." << std::endl;
    return;
}

void InProgressState::finish(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " needs to be reviewed and approved before finishing." << std::endl;
    return;
}

void InProgressState::beginWork(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " cannot begin work as its already in progress." << std::endl;
    return;
}

void InProgressState::beginWork(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " cannot begin work as its already in progress." << std::endl;
    return;
}
#include "InReviewState.h"

InReviewState::InReviewState(WorkFlowComponent *c)
{
    this->context = c;
}

InReviewState::~InReviewState()
{
}

std::string InReviewState::stringStatus() const
{
    return "in review";
}

void InReviewState::review(WorkFlow *wf)
{
}

void InReviewState::review(WorkItem *wi)
{
}

void InReviewState::approve(WorkFlow *wf)
{
    WorkItemState *state = new InReviewState(this->context);
    this->context->setState(state);
    this->context = nullptr;
    std::cout << "WorkFlow " << wf->getName() << " has been approved." << std::endl;
}

void InReviewState::approve(WorkItem *wi)
{
    WorkItemState *state = new InReviewState(this->context);
    this->context->setState(state);
    this->context = nullptr;
    std::cout << "WorkItem " << wi->getName() << " has been approved." << std::endl;
}

void InReviewState::cancel(WorkFlow *wf)
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

void InReviewState::cancel(WorkItem *wi)
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

void InReviewState::finish(WorkFlow *wf)
{
}

void InReviewState::finish(WorkItem *wi)
{
}

void InReviewState::beginWork(WorkFlow *wf)
{
}

void InReviewState::beginWork(WorkItem *wi)
{
}
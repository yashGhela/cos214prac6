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
    std::cout << "WorkFlow " << wf->getName() << " is already being reviewed." << std::endl;
    return;
}

void InReviewState::review(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " is already being reviewed." << std::endl;
    return;
}

void InReviewState::approve(WorkFlow *wf)
{
    std::cout << "Do you wish to approve or reject the current work of " << wf->getName() << "? (A/R)" << std::endl;
    char response;
    std::cin >> response;
    while (response != 'r' && response != 'R' && response != 'A' && response != 'a')
    {
        std::cout << "Please enter A to approve or R to reject" << std::endl;
        std::cin >> response;
    }
    if (response == 'A' || response == 'a')
    {
        WorkItemState *state = new ApprovedState(this->context);
        this->context->setState(state);
        this->context = nullptr;
        std::cout << "WorkFlow " << wf->getName() << " has been approved." << std::endl;
    }
    else if (response == 'R' || response == 'r')
    {
        WorkItemState *state = new InProgressState(this->context);
        this->context->setState(state);
        this->context = nullptr;
        std::cout << "WorkFlow " << wf->getName() << " has been rejected. It has been set back into a state of in progress" << std::endl;
    }
}

void InReviewState::approve(WorkItem *wi)
{
    std::cout << "Do you wish to approve or reject the current work of " << wi->getName() << "? (A/R)" << std::endl;
    char response;
    std::cin >> response;
    while (response != 'r' && response != 'R' && response != 'A' && response != 'a')
    {
        std::cout << "Please enter A to approve or R to reject" << std::endl;
        std::cin >> response;
    }
    if (response == 'A' || response == 'a')
    {
        WorkItemState *state = new ApprovedState(this->context);
        this->context->setState(state);
        this->context = nullptr;
        std::cout << "WorkItem " << wi->getName() << " has been approved." << std::endl;
    }
    else if (response == 'R' || response == 'r')
    {
        WorkItemState *state = new InProgressState(this->context);
        this->context->setState(state);
        this->context = nullptr;
        std::cout << "WorkItem " << wi->getName() << " has been rejected. It has been set back into a state of in progress" << std::endl;
    }
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
    std::cout << "WorkFlow " << wf->getName() << " needs to be approved before its set to finished." << std::endl;
    return;
}

void InReviewState::finish(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " needs to be approved before its set to finished." << std::endl;
    return;
}

void InReviewState::beginWork(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << "'s current work is being reviewed and new work may not start unless rejected." << std::endl;
    return;
}

void InReviewState::beginWork(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << "'s current work is being reviewed and new work may not start unless rejected." << std::endl;
    return;
}
#include "FinishedState.h"

FinishedState::FinishedState(WorkFlowComponent *c)
{
    this->context = c;
}

FinishedState::~FinishedState()
{
}

std::string FinishedState::stringStatus() const
{
    return "finished";
}

void FinishedState::review(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " has finished and can no longer be reviewed." << std::endl;
    return;
}

void FinishedState::review(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " has finished and can no longer be reviewed." << std::endl;
    return;
}

void FinishedState::approve(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " has finished and can no longer be approved." << std::endl;
    return;
}

void FinishedState::approve(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " has finished and can no longer be approved." << std::endl;
    return;
}

void FinishedState::cancel(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " has finished and can no longer be cancelled." << std::endl;
    return;
}

void FinishedState::cancel(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " has finished and can no longer be cancelled." << std::endl;
    return;
}

void FinishedState::finish(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " has already been marked as finished." << std::endl;
    return;
}

void FinishedState::finish(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " has already been marked as finished." << std::endl;
    return;
}

void FinishedState::beginWork(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " has finished and can no longer begin work." << std::endl;
    return;
}

void FinishedState::beginWork(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " has finished and can no longer begin work." << std::endl;
    return;
}
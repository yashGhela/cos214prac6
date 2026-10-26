#include "CancelledState.h"

CancelledState::CancelledState(WorkFlowComponent *c)
{
    this->context = c;
}

CancelledState::~CancelledState()
{
}

std::string CancelledState::stringStatus() const
{
    return "cancelled";
}

void CancelledState::review(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " cannot be reviewed after it has been cancelled" << std::endl;
    return;
}

void CancelledState::review(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " cannot be reviewed after it has been cancelled" << std::endl;
    return;
}

void CancelledState::approve(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " cannot be approved after it has been cancelled" << std::endl;
    return;
}

void CancelledState::approve(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " cannot be approved after it has been cancelled" << std::endl;
    return;
}

void CancelledState::cancel(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " has been cancelled already" << std::endl;
    return;
}

void CancelledState::cancel(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " has been cancelled already" << std::endl;
    return;
}

void CancelledState::finish(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " cannot be finished after it has been cancelled" << std::endl;
    return;
}

void CancelledState::finish(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " cannot be finished after it has been cancelled" << std::endl;
    return;
}

void CancelledState::beginWork(WorkFlow *wf)
{
    std::cout << "WorkFlow " << wf->getName() << " cannot begin work after it has been cancelled" << std::endl;
    return;
}

void CancelledState::beginWork(WorkItem *wi)
{
    std::cout << "WorkItem " << wi->getName() << " cannot begin work after it has been cancelled" << std::endl;
    return;
}
#include "FinishVisitor.h"
#include "WorkFlow.h"
#include "WorkItem.h"

void FinishVisitor::visit(WorkFlow *wf)
{
    if (!wf->isApproved())
    {
        std::cout << "WorkFlow " << wf->getName() << " must first be \033[32m approved\033[0m before setting it to \033[32m finished\033[32m ." << std::endl;
        return;
    }
    std::vector<WorkFlowComponent *> children = wf->getChildren();
    bool allKiddiesDone = true;
    for (auto child : children)
    {
        if (!child->isApproved() && !child->isCancelled() && !child->isFinished())
        {
            allKiddiesDone = false;
            break;
        }
    }
    if (allKiddiesDone)
    {
        wf->setStatus(false, false, false, true, false); // bool progress, bool review, bool approve, bool finish, bool cancel
        std::cout << "WorkFlow " << wf->getName() << " is now \033[32m finished\033[0m ." << std::endl;
    }
    else
    {
        std::cout << "WorkFlow " << wf->getName() << " still has unapproved WorkItem(s). These must be \033[32m approved, finished or cancelled\033[0m first." << std::endl;
    }
}
void FinishVisitor::visit(WorkItem *wi)
{
    if (wi->isApproved())
    {
        wi->setStatus(false, false, false, true, false); // bool progress, bool review, bool approve, bool finish, bool cancel
        std::cout << "WorkItem " << wi->getName() << " is now \033[32m finished\033[0m ." << std::endl;
    }
    else
    {
        std::cout << "WorkItem " << wi->getName() << " must first be \033[32m approved\033[0m before setting it to \033[32m finished\033[0m ." << std::endl;
    }
}
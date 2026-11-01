/**
 * @file BeginWorkVisitor.cpp
 * @brief Implementation of BeginWorkVisitor.
 */
#include "BeginWorkVisitor.h"
#include "WorkFlow.h"
#include "WorkItem.h"

void BeginWorkVisitor::visit(WorkFlow *wf)
{
    if (wf->isCreated())
    {
        wf->setStatus(true, false, false, false, false); // bool progress, bool review, bool approve, bool finish, bool cancel
        std::cout << "WorkFlow " << wf->getName() << " is now \033[32m in progress\033[0m ." << std::endl;
    }
    else
    {
        std::cout << "WorkFlow " << wf->getName() << " can only begin work from the \033[32m Created state\033[0m ." << std::endl;
    }
}
void BeginWorkVisitor::visit(WorkItem *wi)
{
    if (wi->isCreated())
    {
        wi->setStatus(true, false, false, false, false); // bool progress, bool review, bool approve, bool finish, bool cancel
        std::cout << "WorkItem " << wi->getName() << " is now \033[32m in progress\033[0m ." << std::endl;
    }
    else
    {
        std::cout << "WorkItem " << wi->getName() << " can only begin work from the \033[32m Created state\033[0m ." << std::endl;
    }
}
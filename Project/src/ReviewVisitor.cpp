#include "ReviewVisitor.h"
#include "WorkFlow.h"
#include "WorkItem.h"
void ReviewVisitor::visit(WorkFlow *wf)
{
    if (wf->isInProgress())
    {
        wf->setStatus(false, true, false, false, false); // bool progress, bool review, bool approve, bool finish, bool cancel
        std::cout << "WorkFlow " << wf->getName() << " is now \033[32m in review\033[0m ." << std::endl;
    }
    else
    {
        std::cout << "WorkFlow " << wf->getName() << " can only begin to be reviewed from the \033[32m Inprogress state\033[0m ." << std::endl;
    }
}
void ReviewVisitor::visit(WorkItem *wi)
{
    if (wi->isInProgress())
    {
        wi->setStatus(false, true, false, false, false); // bool progress, bool review, bool approve, bool finish, bool cancel
        std::cout << "WorkItem " << wi->getName() << " is now \033[32m in review\033[0m ." << std::endl;
    }
    else
    {
        std::cout << "WorkItem " << wi->getName() << " can only begin to be reviewed from the \033[32m Inprogress state\033[0m ." << std::endl;
    }
}
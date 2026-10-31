#include "CancelVisitor.h"
#include "WorkFlow.h"
#include "WorkItem.h"

void CancelVisitor::visit(WorkFlow *wf)
{
    if (wf->isFinished() || wf->isCancelled())
    {
        std::cout << "WorkFlow " << wf->getName() << " has already been set to a state of \033[32m finished\033[0m or \033[32m cancelled\033[0m ." << std::endl;
        return;
    }
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
        wf->setStatus(false, false, false, false, true); // bool progress, bool review, bool approve, bool finish, bool cancel
    }
}
void CancelVisitor::visit(WorkItem *wi)
{
    if (wi->isFinished() || wi->isCancelled())
    {
        std::cout << "WorkItem " << wi->getName() << " has already been set to a state of \033[32m finished\033[0m or \033[32m cancelled\033[0m ." << std::endl;
        return;
    }
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
        wi->setStatus(false, false, false, false, true); // bool progress, bool review, bool approve, bool finish, bool cancel
    }
}
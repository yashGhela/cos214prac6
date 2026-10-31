#include "ApproveVisitor.h"
#include "WorkFlow.h"
#include "WorkItem.h"

void ApproveVisitor::visit(WorkFlow *wf)
{
    if (!wf->isInReview())
    {
        std::cout << "WorkFlow " << wf->getName() << " can only be approved if its in the \033[32m Review state\033[0m ." << std::endl;
        return;
    }
    std::cout << "Do you wish to approve or reject " << wf->getName() << "? (A/R)" << std::endl;
    char response;
    std::cin >> response;
    while (response != 'A' && response != 'a' && response != 'R' && response != 'r')
    {
        std::cout << "Please enter A or R" << std::endl;
        std::cin >> response;
    }
    if (response == 'A' || response == 'a')
    {
        wf->setStatus(false, false, true, false, false); // bool progress, bool review, bool approve, bool finish, bool cancel
    }
    else
    {
        wf->setStatus(true, false, false, false, false); // bool progress, bool review, bool approve, bool finish, bool cancel
    }
}
void ApproveVisitor::visit(WorkItem *wi)
{
    if (!wi->isInReview())
    {
        std::cout << "WorkItem " << wi->getName() << " can only be approved if its in the \033[32m Review state\033[0m ." << std::endl;
        return;
    }
    std::cout << "Do you wish to approve or reject " << wi->getName() << "? (A/R)" << std::endl;
    char response;
    std::cin >> response;
    while (response != 'A' && response != 'a' && response != 'R' && response != 'r')
    {
        std::cout << "Please enter A or R" << std::endl;
        std::cin >> response;
    }
    if (response == 'A' || response == 'a')
    {
        wi->setStatus(false, false, true, false, false); // bool progress, bool review, bool approve, bool finish, bool cancel
    }
    else
    {
        wi->setStatus(true, false, false, false, false); // bool progress, bool review, bool approve, bool finish, bool cancel
    }
}
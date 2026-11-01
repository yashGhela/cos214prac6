/**
 * @file Member.cpp
 * @brief Implementation of Member.
 */
#include "Member.h"
#include "FinishVisitor.h"

void Member::performWork()
{
    if (!chain || !chain->handleRequest(memberWork))
    {
        std::cout << getName() << ": nobody can handle this item " << memberWork->getName() << "\n";
    }
}

void Member::doWork()
{
    this->performWork();
}

void Member::getWork()
{
    for (auto &entry: assignedWork)
    {
        WorkItem* item = entry.second;
        if (item && item->getParticipant() == nullptr)
        {
            
            item->setParticipant(this);
            memberWork = item;
            return;
        }
    }
}

void Member::submitWork()
{
    FinishVisitor finish;
    memberWork->accept(&finish);
}
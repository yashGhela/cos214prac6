/**
 * @file WorkItem.cpp
 * @brief Implementation of WorkItem.
 */
#include "WorkItem.h"
#include  "VisitorTable.h"
#include "WorkFlowMemento.h"

WorkItem::WorkItem(std::string n)
{
    this->name = n;
    this->assignedParticipant = nullptr;
}

WorkItem::~WorkItem()
{
    this->assignedParticipant = nullptr;
}

void WorkItem::applyAction(const Event& event, WorkFlowComponent*) {
    accept(&visitorFor(event.getAction()));
}


WorkFlowMemento *WorkItem::createMemento()
{
    WorkFlowMemento *m = new WorkFlowMemento();
    std::vector<bool> flags;
    std::map<std::string, std::vector<bool>> snap;
    flags.push_back(isInProgress());
    flags.push_back(isInReview());
    flags.push_back(isApproved());
    flags.push_back(isFinished());
    flags.push_back(isCancelled());
    snap[name] = flags;
    m->setState(snap);
    return m;
}

void WorkItem::setMemento(WorkFlowMemento *m)
{
    auto snap = m->getState();
    auto it = snap.find(name);
    if (it != snap.end())
        setStatus(it->second[0], it->second[1], it->second[2], it->second[3], it->second[4]);
}

Participant *WorkItem::getParticipant()
{
    return this->assignedParticipant;
}

void WorkItem::setParticipant(Participant *p)
{
    this->assignedParticipant = p;
}



std::vector<WorkFlowComponent *> WorkItem::getChildren()
{
    return std::vector<WorkFlowComponent *>();
}

std::string WorkItem::getName()
{
    return this->name;
}

void WorkItem::accept(WorkFlowVisitor *v)
{
    v->visit(this);
}
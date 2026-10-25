#include "WorkItem.h"

WorkItem::WorkItem(std::string n)
{
    this->name = n;
}

WorkItem::~WorkItem()
{
    delete this->state;
    this->assignedParticipant = nullptr;
}

WorkFlowMemento WorkItem::createMemento()
{
    WorkFlowMemento *m = new WorkFlowMemento(state);
    m->setState(this->state);
    m->setParticipant(this->assignedParticipant);
}
void WorkItem::setMemento(WorkFlowMemento *m)
{
    this->assignedParticipant = nullptr;
    this->assignedParticipant = m->getParticipant();
    this->state = m->getState();
}

void WorkItem::setState(WorkItemState *s)
{
    this->state = s;
}
WorkItemState *WorkItem::getState()
{
    return this->state;
}
Participant *WorkItem::getParticipant()
{
    return this->assignedParticipant;
}
void WorkItem::setParticipant(Participant *p)
{
    this->assignedParticipant = p;
}
void WorkItem::update()
{
    // STILL NEEDS TO BE IMPLEMENTED
}

void WorkItem::review()
{
    this->state->review();
}
void WorkItem::approve()
{
    this->state->approve();
}
void WorkItem::cancel()
{
    this->state->cancel();
}
void WorkItem::finish()
{
    this->state->finish();
}
void WorkItem::beginWork()
{
    this->state->beginWork();
}
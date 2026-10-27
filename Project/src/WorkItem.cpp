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

WorkFlowMemento* WorkItem::createMemento() {
    WorkFlowMemento* m = new WorkFlowMemento();
    std::map<std::string, WorkItemState*> snap;
    snap[name] = state;
    m->setState(snap);
    return m;
}

void WorkItem::setMemento(WorkFlowMemento* m) {
    auto snap = m->getState();
    auto it = snap.find(name);
    if (it != snap.end()) setState(it->second);
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

std::string WorkItem::getName()
{
    return this->name;
}

void WorkItem::review(WorkItemState *s)
{
    this->state->review(this);
}

void WorkItem::approve(WorkItemState *s)
{
    this->state->approve(this);
}

void WorkItem::cancel(WorkItemState *s)
{
    this->state->cancel(this);
}

void WorkItem::finish(WorkItemState *s)
{
    this->state->finish(this);
}

void WorkItem::beginWork(WorkItemState *s)
{
    this->state->beginWork(this);
}
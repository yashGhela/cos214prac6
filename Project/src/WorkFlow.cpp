#include "WorkFlow.h"
#include <algorithm>

WorkFlow::WorkFlow(std::string n)
{
    this->name = n;
}

WorkFlow::~WorkFlow()
{
    for (auto *child : this->children)
    {
        delete child;
    }
    delete state;
}

bool WorkFlow::addChild(WorkFlowComponent *c)
{
    auto it = std::find(this->children.begin(), this->children.end(), c);
    if (it != this->children.end())
    {
        return false;
    }
    this->children.push_back(c);
    return true;
}

WorkFlowComponent *WorkFlow::removeChild(WorkFlowComponent *c)
{
    auto it = std::find(this->children.begin(), this->children.end(), c);
    if (it != this->children.end())
    {
        this->children.erase(std::remove(this->children.begin(), this->children.end(), "StateA"), this->children.end());
        return c;
    }
    return nullptr;
}

WorkFlowMemento WorkFlow::createMemento()
{
    WorkFlowMemento *m = new WorkFlowMemento(state);
    m->setState(this->state);
    m->setChildren(this->children);
}

void WorkFlow::setMemento(WorkFlowMemento *m)
{
    for (auto *child : this->children)
    {
        delete child;
    }
    this->children.clear();
    this->state = m->getState();
    this->children = m->getChildren();
}

Iterator *WorkFlow::createIterator()
{
    // STILL NEED TO IMPLEMENT IT
}

void WorkFlow::setState(WorkItemState *s)
{
    this->state = s;
}

WorkItemState *WorkFlow::getState()
{
    return this->state;
}

std::vector<WorkItemState *> WorkFlow::getChildrenState()
{
    std::vector<WorkItemState *> v;
    for (auto *child : this->children)
    {
        v.push_back(child->getState());
    }
    return v;
}

std::string WorkFlow::getName()
{
    return this->name;
}

void WorkFlow::review(WorkItemState *s)
{
    this->state->review(this);
}

void WorkFlow::approve(WorkItemState *s)
{
    this->state->approve(this);
}

void WorkFlow::cancel(WorkItemState *s)
{
    this->state->cancel(this);
}

void WorkFlow::finish(WorkItemState *s)
{
    this->state->finish(this);
}

void WorkFlow::beginWork(WorkItemState *s)
{
    this->state->beginWork(this);
}
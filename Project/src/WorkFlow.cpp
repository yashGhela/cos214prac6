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

WorkFlowMemento* WorkFlow::createMemento() {
    WorkFlowMemento* m = new WorkFlowMemento();
    std::map<std::string, WorkItemState*> snap;
    snap[name] = state;                              

    for (auto* child : getChildren()) {
        WorkFlowMemento* cm = child->createMemento();  
        auto childSnap = cm->getState();
        snap.insert(childSnap.begin(), childSnap.end()); 
        delete cm;
    }

    m->setState(snap);                              
    return m;
}

void WorkFlow::setMemento(WorkFlowMemento* m) {
    auto snap = m->getState();
    auto it = snap.find(name);
    if (it != snap.end()) setState(it->second);      

    for (auto* child : getChildren())
        child->setMemento(m);                        
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

std::vector<WorkFlowComponent *> WorkFlow::getChildren()
{
    std::vector<WorkFlowComponent *> v;
    for (auto *child : this->children)
    {
        v.push_back(child);
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
#include "WorkFlow.h"
#include "WorkFlowVisitor.h"
#include <algorithm>
#include "WorkFlowIterator.h"

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
        this->children.erase(it);
        return c;
    }
    return nullptr;
}

WorkFlowMemento *WorkFlow::createMemento()
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

    for (auto *child : this->children)
    {
        WorkFlowMemento *cm = child->createMemento();
        auto childSnap = cm->getState();
        snap.insert(childSnap.begin(), childSnap.end());
        delete cm;
    }

    m->setState(snap);
    return m;
}

void WorkFlow::setMemento(WorkFlowMemento *m)
{
    auto snap = m->getState();
    auto it = snap.find(name);
    if (it != snap.end())
        setStatus(it->second[0], it->second[1], it->second[2], it->second[3], it->second[4]);

    for (auto *child : this->children)
        child->setMemento(m);
}

Iterator *WorkFlow::createIterator()
{
    return new WorkFlowIterator(this);
}

std::vector<WorkFlowComponent *> WorkFlow::getChildren()
{
    return this->children;
}

std::string WorkFlow::getName()
{
    return this->name;
}

void WorkFlow::accept(WorkFlowVisitor *v)
{
    v->visit(this);
}
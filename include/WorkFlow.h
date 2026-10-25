#ifndef WORKFLOW_H
#define WORKFLOW_H
#include "WorkFlowComponent.h"
#include "Aggregate.h"
class WorkFlow : public WorkFlowComponent, public Aggregate
{
private:
    WorkItemState *state;
    std::string name;
    std::vector<WorkFlowComponent *> children;

public:
    WorkFlow(std::string n);
    ~WorkFlow();
    bool addChild(WorkFlowComponent *c);
    WorkFlowComponent *removeChild(WorkFlowComponent *c);
    WorkFlowMemento createMemento();
    void SetMemento(WorkFlowMemento *m);
    void request();
    void setState(WorkItemState *s);
    WorkItemState *getState();
    Iterator *createIterator();
    std::vector<WorkItemState *> getChildrenState();
};

#endif
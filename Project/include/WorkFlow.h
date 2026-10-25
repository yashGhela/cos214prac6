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
    void setMemento(WorkFlowMemento *m);
    void setState(WorkItemState *s);
    WorkItemState *getState();
    Iterator *createIterator();
    std::vector<WorkItemState *> getChildrenState();
    std::string getName() override;
    void review(WorkItemState *s) override;
    void approve(WorkItemState *s) override;
    void cancel(WorkItemState *s) override;
    void finish(WorkItemState *s) override;
    void beginWork(WorkItemState *s) override;
};

#endif
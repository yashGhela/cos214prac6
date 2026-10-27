#ifndef WORKITEM_H
#define WORKITEM_H
#include "WorkFlowComponent.h"
#include "WorkFlowObserver.h"
class WorkItem : public WorkFlowComponent, public WorkFlowObserver
{
private:
    WorkItemState *state;
    Participant *assignedParticipant;
    std::string name;

public:
    WorkItem(std::string n);
    ~WorkItem();
    WorkFlowMemento* createMemento() override;
    void setMemento(WorkFlowMemento *m) override;
    void setState(WorkItemState *s) override;
    WorkItemState *getState() override;
    Participant *getParticipant();
    void setParticipant(Participant *p);
    void update();
    std::string getName() override;
    void review(WorkItemState *s) override;
    void approve(WorkItemState *s) override;
    void cancel(WorkItemState *s) override;
    void finish(WorkItemState *s) override;
    void beginWork(WorkItemState *s) override;
};

#endif
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
    WorkFlowMemento createMemento();
    void setMemento(WorkFlowMemento *m);
    void request();
    void setState(WorkItemState *s);
    WorkItemState *getState();
    Participant *getParticipant();
    void setParticipant(Participant *p);
    void update();
};

#endif
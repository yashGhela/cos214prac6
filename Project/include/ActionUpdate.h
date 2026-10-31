// this file defines the class that manages updates


#ifndef ACTIONUPDATE_H
#define ACTIONUPDATE_H

#include "WorkFlowObserver.h"
#include "ActionsRegister.h"

class WorkFlowComponent;

class ActionUpdate : public WorkFlowObserver {


protected:
    ActionUpdate(WorkFlowComponent* target, ActionRegistry action)
        : target(target), action(action) {}

    
    virtual void applyAction(const Event& event, WorkFlowComponent* source) = 0;

   
    virtual bool shouldReact(const Event& e, WorkFlowComponent*) const {
        return e.getAction() == action;          
    }
    virtual void afterUpdate(const Event&) {}

    WorkFlowComponent* target;   
    ActionRegistry action;
public:
    ~ActionUpdate() override = default;
// template method: fixed order, cannot be overridden
    void update(const Event& event, WorkFlowComponent* source) final {
        if (!shouldReact(event, source)) return;   // 1. filter
        applyAction(event, source);                // 2. run the visitor
        afterUpdate(event);                        // 3. cleanup/logging
    }

};

#endif
#ifndef ACTIONOBSERVER_H
#define ACTIONOBSERVER_H

#include "WorkFlowObserver.h"

//this templatized observer takes the role of our normal observer to better handle incoming actions while allowing us to still have a general observer for further expansions

class ActionObserver : public WorkFlowObserver {
public:
    ~ActionObserver() override = default;

    void update(const Event& event, WorkFlowComponent* source) final {
        if (!shouldReact(event, source)) return;
        applyAction(event, source);
        afterUpdate(event);
    }

protected:
    virtual void applyAction(const Event& event, WorkFlowComponent* source) = 0;
    virtual bool shouldReact(const Event&, WorkFlowComponent*) const { return true; }
    virtual void afterUpdate(const Event&) {}
};

#endif
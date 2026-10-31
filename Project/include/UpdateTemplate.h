#pragma once

#include "WorkFlowObserver.h"

class WorkFlowObserver;

class UpdateTemplate: public WorkFlowObserver{

    public:
    ~UpdateTemplate() override = default;

    void update(Event event, WorkFlowComponent *source) final;

    protected:

    virtual void applyAction(const Event& event, WorkFlowComponent* source)=0;
    virtual bool shouldReact(const Event& event, WorkFlowComponent* source) const;
    virtual void afterUpdate(const Event& event);


};
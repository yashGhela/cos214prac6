#include "UpdateTemplate.h"

void UpdateTemplate::update(const Event& event, WorkFlowComponent* source){
    if (!shouldReact(event,source)) return;
    applyAction(event, source);
    afterUpdate(event);
}

bool UpdateTemplate::shouldReact(const Event&, WorkFlowComponent*) const { return true; }
void UpdateTemplate::afterUpdate(const Event&) {}
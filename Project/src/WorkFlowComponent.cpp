#include "WorkFlowComponent.h"
#include "VisitorTable.h"

void WorkFlowComponent::applyAction(const Event& event, WorkFlowComponent*) {
    accept(&visitorFor(event.getAction()));
}
#include "ConcreteUpdates.h"
#include "WorkFlowComponent.h"
#include "WorkFlowVisitor.h"
#include "BeginWorkVisitor.h"
#include "ReviewVisitor.h"
#include "ApproveVisitor.h"
#include "CancelVisitor.h"
#include "FinishVisitor.h"

void BeginWorkUpdate::applyAction(const Event&, WorkFlowComponent*) {
    BeginWorkVisitor v;
    target->accept(&v);
}

void InReviewUpdate::applyAction(const Event&, WorkFlowComponent*) {
    ReviewVisitor v;
    target->accept(&v);
}

void ApprovedUpdate::applyAction(const Event&, WorkFlowComponent*) {
    ApproveVisitor v;
    target->accept(&v);
}

void CancelUpdate::applyAction(const Event&, WorkFlowComponent*) {
    CancelVisitor v;
    target->accept(&v);
}

void FinishUpdate::applyAction(const Event&, WorkFlowComponent*) {
    FinishVisitor v;
    target->accept(&v);
}
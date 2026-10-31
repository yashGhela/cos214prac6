#ifndef CONCRETEUPDATES_H
#define CONCRETEUPDATES_H

#include "ActionUpdate.h"

class BeginWorkUpdate : public ActionUpdate {
public:
    explicit BeginWorkUpdate(WorkFlowComponent* t) : ActionUpdate(t, ActionRegistry::BeginWork) {}
protected:
    void applyAction(const Event&, WorkFlowComponent*) override;
};

class InReviewUpdate : public ActionUpdate {
public:
    explicit InReviewUpdate(WorkFlowComponent* t) : ActionUpdate(t, ActionRegistry::InReview) {}
protected:
    void applyAction(const Event&, WorkFlowComponent*) override;
};

class ApprovedUpdate : public ActionUpdate {
public:
    explicit ApprovedUpdate(WorkFlowComponent* t) : ActionUpdate(t, ActionRegistry::Approved) {}
protected:
    void applyAction(const Event&, WorkFlowComponent*) override;
};

class CancelUpdate : public ActionUpdate {
public:
    explicit CancelUpdate(WorkFlowComponent* t) : ActionUpdate(t, ActionRegistry::Cancel) {}
protected:
    void applyAction(const Event&, WorkFlowComponent*) override;
};

class FinishUpdate : public ActionUpdate {
public:
    explicit FinishUpdate(WorkFlowComponent* t) : ActionUpdate(t, ActionRegistry::Finish) {}
protected:
    void applyAction(const Event&, WorkFlowComponent*) override;
};

#endif
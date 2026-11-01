#include "VisitorTable.h"
#include <map>

#include "BeginWorkVisitor.h"
#include "ReviewVisitor.h"
#include "ApproveVisitor.h"
#include "CancelVisitor.h"
#include "FinishVisitor.h"

WorkFlowVisitor& visitorFor(ActionRegistry action){
    static BeginWorkVisitor begin;
    static ReviewVisitor    review;
    static ApproveVisitor   approve;
    static CancelVisitor    cancel;
    static FinishVisitor    finish;

    static const std::map<ActionRegistry, WorkFlowVisitor*> table = {
        {ActionRegistry::BeginWork, &begin},
        {ActionRegistry::InReview,  &review},
        {ActionRegistry::Approved,  &approve},
        {ActionRegistry::Cancel,    &cancel},
        {ActionRegistry::Finish,    &finish},
    };
    return *table.at(action);
}
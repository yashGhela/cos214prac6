
#ifndef VISITORTABLE_H
#define VISITORTABLE_H

#include "ActionsRegister.h"
#include "WorkFlowVisitor.h"

WorkFlowVisitor& visitorFor(ActionRegistry action);

#endif
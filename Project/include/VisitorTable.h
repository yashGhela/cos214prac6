/**
 * @file VisitorTable.h
 * @brief Lookup from an ActionRegistry value to the visitor that performs it.
 */
#ifndef VISITORTABLE_H
#define VISITORTABLE_H

#include "ActionsRegister.h"
#include "WorkFlowVisitor.h"

/**
 * @brief Returns the shared visitor that implements an action.
 *
 * The visitors are function-local statics, so the same instance is
 * returned on every call and nothing has to be deleted.
 *
 * @param action The action to look up.
 * @return Reference to the matching visitor.
 * @throws std::out_of_range if the action has no visitor registered.
 */
WorkFlowVisitor& visitorFor(ActionRegistry action);

#endif
/**
 * @file ActionsRegister.h
 * @brief Enumeration of every action an Event can trigger on a workflow component.
 */
#pragma once

/**
 * @brief The actions that can be attached to an Event.
 *
 * Each value maps (through visitorFor() in VisitorTable.h) to the visitor
 * that performs the matching state transition on a WorkFlowComponent.
 */
enum class ActionRegistry{

    InReview,   ///< Move a component from in-progress to in-review (ReviewVisitor).
    Approved,   ///< Approve or reject a component that is in review (ApproveVisitor).
    BeginWork,  ///< Move a component from created to in-progress (BeginWorkVisitor).
    Cancel,     ///< Cancel a component from any non-final state (CancelVisitor).
    Finish      ///< Finish an approved component (FinishVisitor).

};
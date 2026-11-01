/**
 * @file WorkFlowMemento.h
 * @brief Memento of the Memento pattern.
 */
#ifndef WORKFLOWMEMENTO_H
#define WORKFLOWMEMENTO_H
#include "map"
#include "string"
#include <vector>

class WorkItemState;

/**
 * @brief Snapshot of the state of a workflow tree.
 *
 * The state is a map from component name to its five state flags, in the
 * order in-progress, in-review, approved, finished, cancelled. Its members
 * are private; only the friends WorkFlow and WorkItem (the originators) can
 * read and write the state.
 */
class WorkFlowMemento
{

    friend class WorkFlow;
    friend class WorkItem;

private:
    /** @brief Returns a copy of the stored state. */
    std::map<std::string, std::vector<bool>> getState();
    
    /**
     * @brief Replaces the stored state.
     * @param state Map of component name to its state flags.
     */
    void setState(std::map<std::string, std::vector<bool>> state);
    
    
    std::map<std::string, std::vector<bool>> state; ///< Component name mapped to its five state flags.
};

#endif
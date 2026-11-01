/**
 * @file WorkFlowIterator.h
 * @brief Iterator over the components of a WorkFlow tree.
 */
#ifndef WORKFLOWITERATOR_H
#define WORKFLOWITERATOR_H

#include "Iterator.h"
#include "WorkFlow.h"
#include <map>
#include <vector>

/**
 * @brief Iterator that visits every component below a WorkFlow, including nested ones.
 *
 * The tree is flattened when the iterator is constructed: each child is
 * listed before the children of that child (depth-first, parent first). The
 * iterator does not own the components.
 */
class WorkFlowIterator:public Iterator{
    private:
        std::vector<WorkFlowComponent*> items; ///< Flattened list of components to iterate over.
        int position; ///< Index of the current component. 

        /**
         * @brief Recursively collects the children of a WorkFlow into items.
         * @param wf The workflow whose children are collected.
         */
        void getTheChilds(WorkFlow* wf);
    public:
        
        /**
         * @brief Constructor.
         * @param wf The workflow to iterate over; may be nullptr (giving an empty iteration).
         */
        WorkFlowIterator(WorkFlow* wf);
        
        /** @brief Moves to the next component; does nothing if already done. */
        void next();
        
        /** @brief Moves back to the first component. */ 
        void first();
        
        /** @brief Returns true once the iterator has passed the last component. */
        bool isDone();
        
        /** @brief Returns the current component, or nullptr if the iteration is done. */
        WorkFlowComponent* currentItem();
};

#endif
/**
 * @file VersionIterator.h
 * @brief Iterator over the versions stored in a VersionHistory.
 */
#ifndef VERSIONITERATOR_H
#define VERSIONITERATOR_H

#include "Iterator.h"
#include "WorkFlowMemento.h"
#include <map>

/**
 * @brief Iterator over the mementos of a VersionHistory in ascending version order.
 *
 * The iterator does not own the map or the mementos; they belong to the
 * VersionHistory, which must outlive the iterator.
 */
class VersionIterator:public Iterator{
    private:
        std::map<int, WorkFlowMemento*>* versions; ///< The history's map of versions (not owned).
        std::map<int, WorkFlowMemento*>::iterator current; ///< Position of the current version.   
    public:
        /**
         * @brief Constructor.
         * @param versions Map of version number to memento, owned by a VersionHistory.
         */
        VersionIterator(std::map<int, WorkFlowMemento*>* versions);
        
        /** @brief Destructor; does not delete the map or mementos. */
        ~VersionIterator();
        
        /** @brief Moves to the next version; does nothing if already done. */
        void next();
        
        /** @brief Moves back to the oldest version. */
        void first();
        
        /** @brief Returns true once the iterator has passed the last version. */
        bool isDone();
        
        /** @brief Returns the current memento, or nullptr if the iteration is done. */
        WorkFlowMemento* currentItem();
};

#endif
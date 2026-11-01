/**
 * @file VersionHistory.h
 * @brief Caretaker of the Memento pattern and concrete Aggregate of the Iterator pattern.
 */
#ifndef VERSIONHISTORY_H
#define VERSIONHISTORY_H
#include "WorkFlowMemento.h"
#include "Aggregate.h"
#include "map"

/**
 * @brief Stores saved versions (mementos) of a workflow, keyed by version number.
 *
 * @note VersionHistory owns its mementos and deletes them in its destructor.
 */
class VersionHistory : public Aggregate{

public:
    /** @brief Destructor; deletes every stored memento. */
    ~VersionHistory();

    /**
     * @brief Creates an iterator over the stored versions in ascending version order.
     * @return A new VersionIterator; the caller takes ownership.
     */
    Iterator* createIterator() override;
    
    /**
     * @brief Stores a memento under a version number, replacing any existing entry for that number.
     * @param version Version number.
     * @param memento The memento to store; the history takes ownership.
     */
    void addMemento(int version, WorkFlowMemento* memento);
    
    /**
     * @brief Looks up a stored memento.
     * @param version Version number.
     * @return The memento, or nullptr if the version does not exist.
     */
    WorkFlowMemento *getMemento(int version);

private:
    std::map<int, WorkFlowMemento*> mementos; ///< Stored mementos keyed by version number.

};

#endif
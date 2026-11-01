/**
 * @file Aggregate.h
 * @brief Abstract Aggregate of the Iterator pattern.
 */
#ifndef AGGREGATE_H
#define AGGREGATE_H
#include "Iterator.h"

/**
 * @brief Abstract collection that can create an Iterator over its elements.
 */
class Aggregate{

public:
    /** @brief Virtual destructor. */
    virtual ~Aggregate(){}

    /**
     * @brief Creates an iterator over the aggregate.
     * @return A new Iterator; the caller takes ownership.
     */    
    virtual Iterator* createIterator() = 0;

};

#endif
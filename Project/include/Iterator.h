/**
 * @file Iterator.h
 * @brief Abstract Iterator of the Iterator pattern.
 */
#ifndef ITERATOR_H
#define ITERATOR_H

/**
 * @brief Abstract iterator used to traverse an Aggregate.
 *
 * Concrete iterators (WorkFlowIterator, VersionIterator) add a typed
 * currentItem() of their own.
 */
class Iterator{
    private:
    public:
        /** @brief Moves to the next element; does nothing if already done. */
        virtual void next() =0;

        /** @brief Moves back to the first element. */
        virtual void first()=0;

        /** @brief Returns true once the iterator has passed the last element. */
        virtual bool isDone()=0;
        //virtual void currentItem()=0;

};

#endif
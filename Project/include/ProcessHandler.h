/**
 * @file ProcessHandler.h
 * @brief Base class of the processing step used by handlers in the chain.
 */
#pragma once

class WorkItem;

/**
 * @brief Strategy-style class that knows how to process a WorkItem.
 *
 * A HandlerTemplate holds a ProcessHandler and delegates process() to it.
 */
class ProcessHandler{
    public:
    /** @brief Virtual destructor. */
    virtual ~ProcessHandler() = default;

    /**
     * @brief Processes a work item.
     * @param item The work item to process.
     */
    virtual void process(WorkItem& item);
};

/**
 * @file ProcessHandlerTemplate.h
 * @brief Templated ProcessHandler.
 */
#pragma once

#include "ProcessHandler.h"
#include "WorkItem.h"

class WorkItem;

/**
 * @brief ProcessHandler templated on a type T, so different processing steps can be specialised.
 *
 * The generic version begins work on the item with a BeginWorkVisitor.
 *
 * @tparam T Type used to select a specialisation.
 */
template <typename T>
class ProcessHandlerTemplate: public ProcessHandler{
    public:
    /**
     * @brief Processes a work item by visiting it with a BeginWorkVisitor.
     * @param item The work item to process.
     */
    void process(WorkItem& item) override;

};
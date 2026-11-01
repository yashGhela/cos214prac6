/**
 * @file ProcessHandlerTemplate.cpp
 * @brief Implementation of ProcessHandlerTemplate::process().
 */
#include "ProcessHandlerTemplate.h"

#include "BeginWorkVisitor.h"

template <typename T>
void ProcessHandlerTemplate<T>::process(WorkItem &item)
{
    BeginWorkVisitor beginWork;
    item.accept(&beginWork);
}
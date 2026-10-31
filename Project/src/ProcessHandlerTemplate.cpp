#include "ProcessHandlerTemplate.h"

#include "BeginWorkVisitor.h"

template <typename T>
void ProcessHandlerTemplate<T>::process(WorkItem &item)
{
    BeginWorkVisitor beginWork;
    item.accept(&beginWork);
}
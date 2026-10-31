#include "ProcessHandlerTemplate.h"
#include "WorkItem.h"
#include "BeginWorkVisitor.h"

template <typename T>
void ProcessHandlerTemplate<T>::process(WorkItem &item)
{
    BeginWorkVisitor beginWork;
    item->accept(&beginWork);
}
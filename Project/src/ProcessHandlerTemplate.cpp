#include "ProcessHandlerTemplate.h"
#include "WorkItem.h"
#include "WorkItemState.h"
#include "FinishedState.h"
#include "InProgressState.h"


template <typename T>
void ProcessHandlerTemplate<T>::process(WorkItem& item){

    InProgressState *i = new InProgressState(item);
    item.setState(i);
    


}
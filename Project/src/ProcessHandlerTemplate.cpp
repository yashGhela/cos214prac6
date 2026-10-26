#include "ProcessHandlerTemplate.h"
#include "WorkItem.h"
#include "WorkItemState.h"
#include "FinishedState.h"

void ProcessHandlerTemplate::process(WorkItem& item) override{

    FinishedState *f = new FinishedState();
    item.setState(f);
    


}
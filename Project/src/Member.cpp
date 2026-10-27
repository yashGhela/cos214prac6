#include "Member.h"
#include "FinishedState.h"

void Member::doWork() {
    this->performWork();
}

void Member::getWork() {
    //not really sure what to add here yet till everyone else is done
}

void Member::submitWork() {
    for(auto& [id,item]: assignedWork){
        FinishedState *f = new FinishedState(item);
        
        item->setState(f);
        }
}
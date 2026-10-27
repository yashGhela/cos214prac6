#include "WorkFlowIterator.h"

//nested traversal for sub workflows and work items. checks if workflow and if children is
//workflow with children 

WorkFlowIterator::WorkFlowIterator(WorkFlow* wf){
    position =0;
    if (wf !=nullptr) {
        getTheChilds(wf);
    }
}
void WorkFlowIterator::getTheChilds(WorkFlow* wf) {

    std::vector<WorkFlowComponent*> children = wf->getChildren();
    for (WorkFlowComponent* child :children){
        items.push_back(child);
        WorkFlow* subWorkflow =dynamic_cast<WorkFlow*>(child);

        if(subWorkflow != nullptr){
            getTheChilds(subWorkflow);
        }
    }
}

void WorkFlowIterator::next(){
    if (!isDone()) {
        ++position;
    }
}
void WorkFlowIterator::first(){
    position =0;
}
bool WorkFlowIterator::isDone(){
    return position >= items.size();
}
WorkFlowComponent* WorkFlowIterator::currentItem(){
    if (isDone()) {
        return nullptr;
    }
    return items[position];
}
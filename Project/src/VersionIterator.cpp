#include "../include/VersionIterator.h"

void VersionIterator::next(){
    if(!isDone()) {
        ++current;
    }
}
void VersionIterator::first(){
    current =versions->begin();
}
bool VersionIterator::isDone(){
    return current ==versions->end();
}
WorkFlowMemento* VersionIterator::currentItem(){
    if(isDone()){
        return nullptr;
    }
    return current->second;
}
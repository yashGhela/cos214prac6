/**
 * @file VersionIterator.cpp
 * @brief Implementation of VersionIterator.
 */

#include "../include/VersionIterator.h"

VersionIterator::VersionIterator(std::map<int, WorkFlowMemento*>* v)
    : versions(v)
{
    if (versions)
        current = versions->begin();  
}

VersionIterator::~VersionIterator() {
    // intentionally empty: the map and the mementos belong to VersionHistory
}

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
#include "../include/EventSubject.h"

EventSubject::~EventSubject(){}

void EventSubject::Attach(WorkFlowObserver* observer) {
	//add observer to vector. also checks for multiple
    if (std::find(observerList.begin(),observerList.end(),observer) == observerList.end()){
		observerList.push_back(observer);
	}
}

void EventSubject::Detach(WorkFlowObserver* observer) {
	//remove specified observer from vector
	observerList.erase(std::remove(observerList.begin(), observerList.end(), observer), observerList.end());
}
#include "../include/EventSubject.h"

EventSubject::~EventSubject() {}

void EventSubject::Attach(Event& event, WorkFlowObserver* observer) {
    // Event::addReactor already ignores duplicates
    event.addReactor(observer);
}

void EventSubject::Detach(Event& event, WorkFlowObserver* observer) {
    event.removeReactor(observer);
}

void EventSubject::notify(const Event& event, WorkFlowComponent* source) {
    // copy the list so a reactor can detach itself while reacting
    std::vector<WorkFlowObserver*> snapshot = event.getReactors();
    for (WorkFlowObserver* observer : snapshot) {
        observer->update(event, source);
    }
}
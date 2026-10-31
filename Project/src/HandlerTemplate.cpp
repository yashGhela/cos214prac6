#include "WorkHandler.h"
#include "HandlerTemplate.h"
#include "VisitorTable.h"


template <typename T>
bool HandlerTemplate<T>::handleRequest(WorkItem& item){

    if(item){
        process(item);
        return true;
    
    }

    return WorkHandler::successor ? WorkHandler::successor->handleRequest(item) : false;

}

template <typename T>
void HandlerTemplate<T>::applyAction(const Event& event, WorkFlowComponent* source) {
    accept(&visitorFor(event.getAction()));
}
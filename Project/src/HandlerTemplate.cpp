#include "WorkHandler.h"


bool WorkHandler::handleRequest(WorkItem& item) override{

    if(item){
        process(item);
        return true;
    
    }

    return successor ? successor->handleRequest(item) : false;

}
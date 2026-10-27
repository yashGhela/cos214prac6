#include <iostream>
#include "WorkFlow.h"
#include "WorkItem.h"
#include "CreatedState.h"
#include "TIVIDYFacade.h"
using namespace std;

int main(){

    TIVIDYFacade *facade = new TIVIDYFacade();

    WorkFlow* wf = new WorkFlow("KeyFraming");

    WorkItem* a = new WorkItem("Map out the time stops");

}
#include "RolesBasic.h"
#include "Participant.h"
#include <string>
#include <iostream>
#include <cstdlib>//for rand()
using namespace std;

RolesBasic::RolesBasic(Participant* decorated){
    this->decorated = decorated;
    roleName = "LOW_WORKER";
    roleLevel = 5;
}

RolesBasic::~RolesBasic(){//destructor to handle assigned pointer of decorated
    delete decorated;
}

int RolesBasic::noWorkChance(){
    return rand()%101;//give random number between 0 and 100
}
//maybe we can have getName() and name member to make participants more distinct from each other
void RolesBasic::doWork(){
    int workRoll = noWorkChance();
    if(workRoll<=25){//if chance got is 25% or less then work attempt fails
        cout<<"Seems \033[95m"<<roleName<<"\033[0m "<<decorated->getName()<<" of"<<"\033[95m"<<" rank "<<roleLevel<<"\033[0m has not done any work!"<<endl;
    }else{
        cout<<"\033[95m"<<roleName<<"\033[0m "<<decorated->getName()<<" of"<<"\033[95m"<<" rank "<<roleLevel<<"\033[0m is trying to work!"<<endl;
        decorated->doWork();
    }
}

void RolesBasic::getWork(){
    cout<<"\033[95m"<<roleName<<"\033[0m "<<decorated->getName()<<" of"<<"\033[95m"<<" rank "<<roleLevel<<"\033[0m is trying to find some more work to do."<<endl;
    decorated->getWork();
}

void RolesBasic::submitWork(){
    cout<<"\033[95m"<<roleName<<"\033[0m "<<decorated->getName()<<" of"<<"\033[95m"<<" rank "<<roleLevel<<"\033[0m is attempting to submit their work."<<endl;
    decorated->submitWork();
}
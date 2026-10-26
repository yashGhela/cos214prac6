#include "RolesManager.h"
#include "Participant.h"
#include <string>
#include <iostream>
#include <cstdlib>
using namespace std;

RolesManager::RolesManager(Participant* decorated){
    this->decorated = decorated;
    roleName = "MANAGER";
    roleLevel = 10;
}

RolesManager::~RolesManager(){
    delete decorated;
}

int RolesManager::distractChance(){
    return rand()%101;
}
//maybe we can have getName() and name member to make participants more distinct from each other
void RolesManager::doWork(){
    if(distractChance()<=4){
        cout<<"Seems like "<<roleName<<" "<<decorated->getName()<<" of rank "<<roleLevel<<" got distracted with meetings!"<<endl;
    }else{
        cout<<"\033[96m"<<roleName<<"\033[0m "<<decorated->getName()<<" of rank \033[96m"<<roleLevel<<"\033[0m is working on difficult tasks!"<<endl;
        decorated->doWork();
    }
}

void RolesManager::getWork(){
    cout<<roleName<<" "<<decorated->getName()<<" of rank "<<roleLevel<<" is checking if there is any difficult jobs to do."<<endl;
    decorated->getWork();
}

void RolesManager::submitWork(){
    cout<<roleName<<" "<<decorated->getName()<<" of rank "<<roleLevel<<" is attempting to submit their work for approval."<<endl;
    decorated->submitWork();
}
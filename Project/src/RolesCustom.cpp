#include "RolesCustom.h"
#include "Participant.h"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;
//will add error checking later just basic work for now
RolesCustom::RolesCustom(Participant* decorated){
    this->decorated = decorated;
    string gotContent = "";
    cout<<"Enter name for the role:"<<endl;
    cin>>gotContent;
//TODO: verify gotContent does not contain anythig of LOW_WORKER, MANAGER or APPROVER else repeat input request
    this->roleName = gotContent;
    gotContent = "";
    int roleLevel = -1;
    stringstream lvlConvert;
    cout<<"Enter the level that this role will have:"<<endl;
    cin>>gotContent;
//TODO: verify gotContent does not contain anything lower than 1 else repeat input request
    lvlConvert<<gotContent;
    lvlConvert>>roleLevel;
    this->roleLevel = roleLevel;
    gotContent = "";
    cout<<"Enter what this role 'says':"<<endl;
    cin>>gotContent;
//TODO: verify gotContent is not empty or filled with only spaces else repeat input request
    this->addedOutput = gotContent;
}

RolesCustom::RolesCustom(Participant* decorated, string addedOutput, string roleName, int roleLevel){
    this->decorated = decorated;
    this->roleName = roleName;
    this->roleLevel = roleLevel;
    this->addedOutput = addedOutput;
}

RolesCustom::~RolesCustom(){
    if(!decorated){
        return;
    }
    delete decorated;
}
//maybe we can have getName() and name member to make participants more distinct from each other
void RolesCustom::doWork(){
    cout<<roleName<<" "<<decorated->getName()<<" of rank "<<roleLevel<<" is trying to work."<<endl;
    cout<<addedOutput<<endl;
    decorated->doWork();
}

void RolesCustom::getWork(){
    cout<<roleName<<" "<<decorated->getName()<<" of rank "<<roleLevel<<" is trying to find some work to do."<<endl;
    decorated->getWork();
}

void RolesCustom::submitWork(){
    cout<<roleName<<" "<<decorated->getName()<<" of rank "<<roleLevel<<" is trying to submit their work for approval."<<endl;
}
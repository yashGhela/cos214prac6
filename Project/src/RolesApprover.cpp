/**
 * @file RolesApprover.cpp
 * @brief Implementation of RolesApprover.
 */
#include "RolesApprover.h"
#include "Participant.h"
#include <string>
#include <iostream>
using namespace std;

RolesApprover::RolesApprover(Participant* decorated):Roles(decorated){
    this->decorated = decorated;
    roleName = "APPROVER";
    roleLevel = 15;
}

RolesApprover::~RolesApprover(){
    delete decorated;
}
//maybe we can have getName() and name member to make participants more distinct from each other
void RolesApprover::doWork(){
    cout<<"\033[93m"<<roleName<<"\033[0m "<<decorated->getName()<<" of rank \033[93m"<<roleLevel<<"\033[0m is going through the approval process!";
    decorated->doWork();
}

void RolesApprover::getWork(){
    cout<<"\033[93m"<<roleName<<"\033[0m "<<decorated->getName()<<" of rank \033[93m"<<roleLevel<<"\033[0m is checking for more apporval worok.";
    decorated->getWork();
}

void RolesApprover::submitWork(){//maybe sends to different approver for approval
    cout<<"\033[93m"<<roleName<<"\033[0m "<<decorated->getName()<<" of rank \033[93m"<<roleLevel<<"\033[0m is attempting to submit work for co-approval."<<endl;
    decorated->submitWork();
}
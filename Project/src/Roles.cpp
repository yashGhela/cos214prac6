/**
 * @file Roles.cpp
 * @brief Implementation of the Roles decorator.
 */
#include "Roles.h"
#include "Participant.h"
#include <string>
#include <iostream>
using namespace std;

Roles::Roles(Participant* p)
    : Participant(p->getName()),   // base needs a name, so reuse the wrapped one
      decorated(p),
      roleName(""),
      roleLevel(0) {}

Roles::~Roles(){}
// \033[91m makes the terminal font colour bright red + \033[0m resets the colour back to its default
void Roles::giveNullError(){
    cout<<"\t\t"<<"\033[91m"<<"====== ERROR ======"<<endl;
    cout<<"This participant is null, please ensure you use more specialised versions of the roles OR make your own custom role(s) \033[0m"<<endl;
}

void Roles::doWork(){
    giveNullError();
    cout<<"\033[91m"<<roleName<<"\033[0m of "<<"\033[91m"<<roleLevel<<"\033[0m is doing some work?"<<endl;
    decorated->doWork();
}

void Roles::getWork(){
    giveNullError();
    cout<<"\033[91m"<<roleName<<"\033[0m of "<<"\033[91m"<<roleLevel<<"\033[0m cannot be found to give work"<<endl;
    decorated->getWork();
}

void Roles::submitWork(){
    giveNullError();
    cout<<"\033[91m"<<roleName<<"\033[0m of "<<"\033[91m"<<roleLevel<<"\033[0m cannot submit null work"<<endl;
    decorated->submitWork();
}

bool Roles::hasRole(const std::string& role) const {
    return decorated->hasRole(role);
}

int Roles::getWorkLoad() const {
    return decorated->getWorkLoad();
}
/**
 * @file Team.cpp
 * @brief Implementation of Team.
 */
#include "Team.h"
#include "Participant.h"

void Team::addMember(Participant* p) {
    if (!p) return;
    teamMembers[p->getName()] = p;
}


void Team::removeMember(const std::string& name) { teamMembers.erase(name); }


void Team::doWork() {
    for(const auto& entry:teamMembers){
        Participant* t = entry.second;
        t->doWork();
    }
}

void Team::getWork() {
    for(const auto& entry:teamMembers){
        Participant* t = entry.second;
        t->getWork();
    }
}

void Team::submitWork() {
    for(const auto& entry:teamMembers){
        Participant* t = entry.second;
        t->submitWork();
    }
}

// bool hasRole(const std::string& role)  {
//         for (auto& [_, m] : teamMembers) if (m->hasRole(role)) return true;
//         return Participant::hasRole(role);
//     }

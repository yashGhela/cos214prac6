#include "Team.h"


void Team::addMember(std::unique_ptr<Participant> p) {
        teamMembers_[p->name()] = std::move(p);
    }
void Team::removeMember(const std::string& name) { teamMembers_.erase(name); }


void Team::doWork() override{
    for(const auto& [_,t]:teamMembers){
        t->doWork();
    }
}

void Team::getWork() override{
    for(const auto& [_,t]:teamMembers){
        t->getWork();
    }
}

void Team::submitWork() override{
    for(const auto& [_,t]:teamMembers){
        t->submitWork();
    }
}

bool hasRole(const std::string& role) const override {
        for (auto& [_, m] : teamMembers_) if (m->hasRole(role)) return true;
        return Participant::hasRole(role);
    }

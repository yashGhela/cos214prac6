#pragma once

#include "Participant.h"

class Team : public Participant
{
private:
    std::map<std::string, Participant *> teamMembers;
    std::string teamName;

public:
    Team(std::string name) : Participant(name) {};

    void addMember(Participant *p);

    void removeMember(const std::string &name);

    void getWork() override;

    void doWork() override;

    void submitWork() override;
    const std::map<std::string, Participant*>& getMembers() const { return teamMembers; }
};
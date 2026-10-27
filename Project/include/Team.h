#pragma once

#include "Participant.h"


class Team: public Participant{
    private:
    std::map<std::string, std::unique_ptr<Participant>> teamMembers;

    public:



    void addMember(std::unique_ptr<Participant> p);

    void removeMember(const std::string& name);

   void getWork() override;

   void doWork() override;

   void submitWork() override;

 
   
};
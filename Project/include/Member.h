#pragma once
#include "Participant.h"

class Member: public Participant{

    WorkItem* memberWork;

    public:


    Member(std::string name):Participant(name){};
    void performWork() override;
    void doWork() override;
    void getWork() override;
    void submitWork() override;

};
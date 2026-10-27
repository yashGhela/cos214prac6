#pragma once
#include "Participant.h"

class Member: public Participant{

    public:


    Member(std::string name):Participant(name){};;
    void doWork() override;
    void getWork() override;
    void submitWork() override;

};
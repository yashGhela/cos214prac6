#pragma once
#include "Participant.h"

class Member: public Participant{

    public:


    void doWork() override;
    void getWork() override;
    void submitWork() override;

}
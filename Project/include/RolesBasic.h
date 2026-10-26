#ifndef ROLES_BASIC_H
#define ROLES_BASIC_H
#include "Roles.h"
#include <string>
using namespace std;

class Participant;
class RolesBasic: public Roles{
    private:
        int noWorkChance();
    public:
        RolesBasic(Participant* decorated);
        ~RolesBasic();
        void doWork();
        void getWork();
        void submitWork();
};

#endif
#ifndef ROLES_MANAGER_H
#define ROLES_MANAGER_H
#include "Roles.h"
#include <string>
using namespace std;

class RolesManager: public Roles{
    private:
        int distractChance();
    public:
        RolesManager(Participant* decorated);
        ~RolesManager();
        void doWork();
        void getWork();
        void submitWork();
};

#endif
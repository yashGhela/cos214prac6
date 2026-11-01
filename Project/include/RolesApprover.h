#ifndef ROLES_APPROVER_H
#define ROLES_APPROVER_H
#include "Roles.h"
#include <string>
using namespace std;

class Participant;
class RolesApprover: public Roles{
    public:
        RolesApprover(Participant* decorated);
        ~RolesApprover();
        void doWork();
        void getWork();
        void submitWork();
};

#endif
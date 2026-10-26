#ifndef ROLES_CUSTOM_H
#define ROLES_CUSTOM_H
#include "Roles.h"
#include <string>
using namespace std;

class Participant;
class RolesCustom: public Roles{
    private:
        string addedOutput;
        string getUserInp();//used to get what user wants
    public:
        RolesCustom(Participant* decorated);//receive addedOutput + roleName + roleLevel from user inp as constructor called
        RolesCustom(Participant* decorated, string addedOutput, string roleName, int roleLevel);//used for testing show custom build
        ~RolesCustom();
        void getWork();
        void doWork();
        void submitWork();
};

#endif
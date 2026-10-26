#ifndef ROLES_H
#define ROLES_H
#include "Participant.h"
#include <string>
using namespace std;

class Participant;
class Roles: public Participant{
    protected:
        Participant* decorated;//pointer to the participant who is being decorated
        string roleName;//name of the person's role
        int roleLevel;//level to det when to shift work, incs in 5s
    private:
        void giveNullError();//outputs "error" message if this is used instead of derived roles
    public:
        Roles();
        virtual ~Roles();
    // basically these are not supposed to do anything so handle it as such if this is used???
        virtual void getWork();
        virtual void doWork();
        virtual void submitWork();
};

#endif
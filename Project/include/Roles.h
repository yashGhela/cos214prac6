#ifndef ROLES_H
#define ROLES_H

#include <string>
#include "Participant.h"

class Roles : public Participant {
protected:
    Participant* decorated;   // the participant being decorated
    std::string roleName;     // name of this role
    int roleLevel;            // level to decide when to shift work, increases in 5s

public:
    explicit Roles(Participant* p);
    virtual ~Roles();

    // default behaviour: pass through to the wrapped participant
    void getWork() override;
    void doWork() override;
    void submitWork() override;
    bool hasRole(const std::string& role) const override;
    int getWorkLoad() const override;
private:
void giveNullError();
};

#endif
#pragma once 
#include "WorkItem.h"
#include <string>

class WorkItem;

class Participant{

    protected:
    std::map<int,WorkItem*> assignedWork;
    std::string part_name;
    std::map<std::string, bool> part_roles;

    

    public:

    Participant(std::string name): part_name((name)){};
    virtual ~Participant();
    virtual void doWork()=0;
    virtual void getWork()=0;
    virtual void submitWork()=0;
    virtual bool hasRole(const std::string &role) const {return roles.count(role);};
    virtual int getWorkLoad() const {return static_cast<int>(assignedWork.size());};
    const std::string& name() const {return part_name;};
    

}
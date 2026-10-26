#pragma once 
#include "WorkItem.h"
#include <string>
#include "WorkHandler.h"

class WorkItem;

class Participant{

    protected:
    std::map<int,WorkItem*> assignedWork;
    std::string part_name;
    WorkHandler* chain;
    std::map<std::string, bool> part_roles;

    

    public:

    Participant(std::string name): part_name((name)){};
    virtual ~Participant();
    void performWork(){
        for(auto& [id,item]: assignedWork_){
            if(!chain || !chain->handleRequest(*item)){
                std::cout<<name<<": nobody can handle this item "<<id <<"\n";
            }
        }
    }

    void setChain(WorkHandler* head) { chain_ = head; }
    void assignWork(WorkItem* item)  { assignedWork_[item->id()] = item; }

    virtual void doWork()=0;
    virtual void getWork()=0;
    virtual void submitWork()=0;
    virtual bool hasRole(const std::string &role) const {return roles.count(role);};
    virtual int getWorkLoad() const {return static_cast<int>(assignedWork.size());};
    const std::string& name() const {return part_name;};
    

}
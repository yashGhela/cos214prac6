#pragma once 
#include "WorkItem.h"
#include <string>
#include "WorkHandler.h"

class WorkItem;

class Participant{

    protected:
    std::map<std::string,WorkItem*> assignedWork;
    std::string part_name;
    WorkHandler* chain;
    std::map<std::string, bool> part_roles;

    

    public:

    Participant(std::string name): part_name((name)){};
    virtual ~Participant();
    virtual void performWork(){
        for(auto& entry: assignedWork){
            WorkItem* item = entry.second;
            if(!chain || !chain->handleRequest(item)){
                std::cout<<getName()<<": nobody can handle this item " <<"\n";
            }
        }
    }




    std::string getName() const { return part_name; }
    void setChain(WorkHandler* head) { chain = head; }
    void assignWork(WorkItem* item)  { assignedWork[item->getName()] = item; }

    virtual void doWork()=0;
    virtual void getWork()=0;
    virtual void submitWork()=0;
    virtual bool hasRole(const std::string &role) const {return part_roles.count(role);};
    virtual int getWorkLoad() const {return static_cast<int>(assignedWork.size());};
    const std::string& name() const {return part_name;};
    

};
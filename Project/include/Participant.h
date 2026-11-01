/**
 * @file Participant.h
 * @brief Component of the Participant Composite structure (also the Decorator component).
 */
#pragma once 
#include "WorkItem.h"
#include <string>
#include "WorkHandler.h"

class WorkItem;

/**
 * @brief Abstract person or group that can be assigned work.
 *
 * Component of the Participant Composite (Member is the leaf, Team the
 * composite) and also the component that Roles decorate.
 */
class Participant{

    protected:
    std::map<std::string,WorkItem*> assignedWork; ///< Work items assigned to this participant, keyed by item name (not owned).
    std::string part_name; ///< Name of the participant.
    WorkHandler* chain; ///< Head of the handler chain used to process work (not owned).
    std::map<std::string, bool> part_roles; ///< Roles held by the participant, keyed by role name.

    

    public:

    /**
     * @brief Constructor.
     * @param name Name of the participant.
     */
    Participant(std::string name): part_name((name)){};
    
    /** @brief Virtual destructor. */
    virtual ~Participant(){};
    
    /**
     * @brief Sends every assigned work item to the handler chain.
     *
     * Prints a message for each item that no handler (or no chain) can handle.
     */
    virtual void performWork(){
        for(auto& entry: assignedWork){
            WorkItem* item = entry.second;
            if(!chain || !chain->handleRequest(item)){
                std::cout<<getName()<<": nobody can handle this item " <<"\n";
            }
        }
    }


    /** @brief Returns the name of the participant. */
    std::string getName() const { return part_name; }
    
    /**
     * @brief Sets the head of the handler chain used to process work.
     * @param head First handler in the chain (not owned).
     */
    void setChain(WorkHandler* head) { chain = head; }
    
    /**
     * @brief Assigns a work item to the participant, replacing any item with the same name.
     * @param item The work item to assign (not owned).
     */
    void assignWork(WorkItem* item)  { assignedWork[item->getName()] = item; }

    /** @brief Does the participant's work. */
    virtual void doWork()=0;
    
    /** @brief Picks up work for the participant. */
    virtual void getWork()=0;
    
    /** @brief Submits the participant's finished work. */
    virtual void submitWork()=0;
    
    /**
     * @brief Checks whether the participant holds a role.
     * @param role Name of the role.
     * @return true if the participant holds the role.
     */
    virtual bool hasRole(const std::string &role) const {return part_roles.count(role);};
    
    /** @brief Returns the number of work items assigned to the participant. */
    virtual int getWorkLoad() const {return static_cast<int>(assignedWork.size());};
    
    /** @brief Returns a reference to the participant's name. */
    const std::string& name() const {return part_name;};
    

};
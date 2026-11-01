/**
 * @file RolesApprover.h
 * @brief Concrete decorator that makes a Participant an approver.
 */
#ifndef ROLES_APPROVER_H
#define ROLES_APPROVER_H
#include "Roles.h"
#include <string>
using namespace std;

class Participant;

/**
 * @brief Decorates a Participant with the APPROVER role (level 15).
 *
 * @note The decorator takes ownership of the wrapped participant and deletes it in its destructor.
 */
class RolesApprover: public Roles{
    public:
        /**
         * @brief Constructor.
         * @param decorated The participant to decorate; ownership passes to the approver.
         */
        RolesApprover(Participant* decorated);
        
        /** @brief Destructor; deletes the wrapped participant. */
        ~RolesApprover();
        
        /** @brief Announces the approval process then lets the wrapped participant do its work. */
        void doWork();
        
        /** @brief Announces a check for more approval work then lets the wrapped participant get work. */
        void getWork();
        
        /** @brief Announces a co-approval submission then lets the wrapped participant submit its work. */
        void submitWork();
};

#endif
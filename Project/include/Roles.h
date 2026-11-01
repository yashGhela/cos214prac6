/**
 * @file Roles.h
 * @brief Decorator of the Decorator pattern that adds a role to a Participant.
 */
#ifndef ROLES_H
#define ROLES_H

#include <string>
#include "Participant.h"

/**
 * @brief Base decorator that wraps a Participant and gives it a role.
 *
 * By default every operation is passed through to the wrapped participant,
 * after printing an error that this base role is unspecialised. Use a more
 * specialised role such as RolesApprover, or make a custom role.
 */
class Roles : public Participant {
protected:
    Participant* decorated; ///< The participant being decorated.
    std::string roleName; ///< Name of this role.
    int roleLevel; ///< Level used to decide when to shift work; increases in 5s.

public:
    /**
     * @brief Constructor.
     * @param p The participant to decorate; its name is reused for the decorator.
     */
    explicit Roles(Participant* p);
    
    /** @brief Virtual destructor. */
    virtual ~Roles();

    /** @brief Prints an error then passes the call through to the wrapped participant. */
    void getWork() override;
    
    /** @brief Prints an error then passes the call through to the wrapped participant. */
    void doWork() override;
    
    /** @brief Prints an error then passes the call through to the wrapped participant. */
    void submitWork() override;
    
    /**
     * @brief Checks whether the wrapped participant holds a role.
     * @param role Name of the role.
     * @return true if the wrapped participant holds the role.
     */
    bool hasRole(const std::string& role) const override;
    
    /** @brief Returns the work load of the wrapped participant. */
    int getWorkLoad() const override;
private:
/** @brief Prints a coloured error saying the participant is null/unspecialised. */
void giveNullError();
};

#endif
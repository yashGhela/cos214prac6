/**
 * @file Team.h
 * @brief Composite of the Participant Composite structure.
 */
#pragma once

#include "Participant.h"

/**
 * @brief A group of participants (members or other teams) that acts as one participant.
 *
 * Every operation is forwarded to each team member.
 *
 * @note The team does not own its members.
 */
class Team : public Participant
{
private:
    std::map<std::string, Participant *> teamMembers;
    std::string teamName;

public:
    /**
     * @brief Constructor.
     * @param name Name of the team.
     */
    Team(std::string name) : Participant(name) {};

    /**
     * @brief Adds a member to the team, replacing any member with the same name.
     * @param p The participant to add; nullptr is ignored.
     */
    void addMember(Participant *p);

    /**
     * @brief Removes a member from the team.
     * @param name Name of the member to remove; does nothing if there is no such member.
     */
    void removeMember(const std::string &name);

    /** @brief Calls getWork() on every member. */
    void getWork() override;

    /** @brief Calls doWork() on every member. */
    void doWork() override;

    /** @brief Calls submitWork() on every member. */
    void submitWork() override;
    const std::map<std::string, Participant*>& getMembers() const { return teamMembers; }
};
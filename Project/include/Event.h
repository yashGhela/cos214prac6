/**
 * @file Event.h
 * @brief Event and EventRegistry, the data classes used by the Observer pattern.
 */
#ifndef EVENT_H
#define EVENT_H
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "ActionsRegister.h"

class WorkFlowObserver;
class WorkFlowComponent;

/**
 * @brief A named event that carries an action and the list of observers reacting to it.
 *
 * Users can create their own events with a unique name and an ActionRegistry
 * value and pass them around seamlessly. An EventSubject attaches observers
 * to an Event and notifies them when the event fires.
 *
 * @note The Event does not own its observers; it only stores raw pointers.
 */
class Event
{
private:
    std::string name;
    std::vector<WorkFlowObserver *> reactors;
    ActionRegistry action;

public:
    /**
     * @brief Constructs an event.
     * @param eventName Name of the event.
     * @param act Action that observers should apply when the event fires.
     */
    Event(std::string eventName, ActionRegistry act) : name(eventName), action(act) {};

    /** @brief Returns the action carried by this event. */
    ActionRegistry getAction() const { return action; }
    
    /** @brief Returns the name of this event. */
    std::string getName() const { return name; };
    
    /**
     * @brief Adds an observer to the list of reactors.
     *
     * An observer that is already registered is ignored, so each observer
     * reacts at most once per notification.
     *
     * @param w Observer to add (not owned, must outlive the event or be removed first).
     */
    void addReactor(WorkFlowObserver *w) 
    {
        if (w != nullptr && std::find(reactors.begin(), reactors.end(), w) == reactors.end())
        {
            reactors.push_back(w);
        }
    }

    /**
     * @brief Removes an observer from the list of reactors.
     *
     * Does nothing if the observer is not registered.
     *
     * @param w Observer to remove.
     */
    void removeReactor(WorkFlowObserver *w)
    {
        reactors.erase(std::remove(reactors.begin(), reactors.end(), w), reactors.end());
    }

    /** @brief Returns the observers currently reacting to this event. */
    const std::vector<WorkFlowObserver *> &getReactors() const { return reactors; }
};

/**
 * @brief Registry of the event names that have been defined.
 *
 * Names are normalised so that "kitchen closed", "kitchen_closed" and
 * "KITCHEN_CLOSED" all refer to the same event.
 */
class EventRegistry
{
private:
    std::map<std::string, Event> events;

public:
    /**
     * @brief Normalises an event name.
     *
     * Spaces become underscores and letters become upper case, so that
     * kitchen_closed and KITCHEN_CLOSED are the same thing.
     *
     * @param n Name to normalise.
     * @return The normalised name.
     */
    static std::string normalise(std::string n)
    {
        for (char &c : n)
            c = (c == ' ') ? '_' : std::toupper(static_cast<unsigned char>(c));
        return n;
    }

    /**
     * @brief Defines a new event.
     * @param name Name of the event (normalised before it is stored).
     * @param action Action the event triggers.
     * @return true if the event was defined, false if one with that name already exists.
     */
    bool define(const std::string &name, ActionRegistry action)
    {
        std::string key = normalise(name);
        return events.emplace(key, Event(key, action)).second;
    }

    /**
     * @brief Checks whether an event has been defined.
     * @param name Name of the event (normalised before the lookup).
     * @return true if the event exists.
     */
    bool exists(const std::string &name) const { return events.count(normalise(name)) > 0; }
    
    /** @brief Returns the normalised names of all defined events. */
    std::vector<std::string> all() const {
        std::vector<std::string> names;
        for (const auto& kv : events) names.push_back(kv.first);
        return names;
    }

    /**
     * @brief Looks up a defined event.
     *
     * Returns a pointer to the registry's own Event, so reactors attached to it
     * are kept. Do not copy the Event, a copy has its own separate reactor list.
     *
     * @param name Name of the event (normalised before the lookup).
     * @return Pointer to the event, or nullptr if no event with that name exists.
     */
    Event *get(const std::string &name)
    {
        std::map<std::string, Event>::iterator it = events.find(normalise(name));
        return it != events.end() ? &it->second : nullptr;
    }
};

#endif
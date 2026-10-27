#include <iostream>
#include <vector>
#include <string>
#include <set>
//this function lets users create their own events w unique name and id
//a user can then create their own event class anywhere and pass it seamlessly

class Event{
    private:
    std::string name;

    public:
    Event(std::string eventName):name(eventName){};
    std::string getName() const {return name;};
};

class EventRegistry{
    private:
    std::set<std::string> events;

    public:

    static std::string normalise(std::string n) {
        for (char& c : n) c = (c == ' ') ? '_' : std::toupper(static_cast<unsigned char>(c));
        return n;
    }//this function jsut sets it so kitchen_closed and KITCHEN_CLOSED are the same thing
    bool define(const std::string& name) { return events.insert(normalise(name)).second; }
    bool exists(const std::string& name) const { return events.count(normalise(name)) > 0; }
    const std::set<std::string>& all() const { return events; }


};
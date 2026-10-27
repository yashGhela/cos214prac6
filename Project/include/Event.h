#include <iostream>
#include <vector>
#include <string>

//this function lets users create their own events w unique name and id
//a user can then create their own event class anywhere and pass it seamlessly

class Event{
    private:
    std::string name;

    public:
    Event(std::string eventName):name(eventName){};
    std::string getName() const {return name;};
};
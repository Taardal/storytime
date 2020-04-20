#include "Event.h"
#include <sstream>

namespace storytime
{
    Event::Event(std::string_view name, EventType type)
            : type(type), name(name), handled(false)
    {
    }

    std::string_view Event::GetName() const
    {
        return name;
    }

    EventType Event::GetType() const
    {
        return type;
    }

    bool Event::IsHandled() const
    {
        return handled;
    }

    void Event::SetHandled(bool handled)
    {
        this->handled = handled;
    }

    std::string Event::ToString() const
    {
        std::stringstream ss;
        ss << name << "{}";
        return ss.str();
    }

}

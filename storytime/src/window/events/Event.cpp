#include "Event.h"
#include <sstream>

namespace storytime {

    Event::Event(std::string_view name, EventType type) : type(type), name(name), handled(false) {
    }

    std::string_view Event::getName() const {
        return name;
    }

    EventType Event::getType() const {
        return type;
    }

    bool Event::isHandled() const {
        return handled;
    }

    void Event::setHandled(bool handled) {
        this->handled = handled;
    }

    std::string Event::toString() const {
        std::stringstream ss;
        ss << name << "{}";
        return ss.str();
    }

}

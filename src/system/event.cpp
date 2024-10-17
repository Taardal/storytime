#include "system/event.h"

namespace Storytime {
    Event::Event(EventType type, const std::string& name) : type(type), name(name) {
    }

    std::string Event::get_name() const {
        return name;
    }

    EventType Event::get_type() const {
        return type;
    }

    std::string Event::to_string() const {
        std::stringstream ss;
        ss << name << "{}";
        return ss.str();
    }
}

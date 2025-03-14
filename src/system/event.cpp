#include "system/event.h"

namespace Storytime {
    Event::Event(EventType event_type, const std::string& event_type_name)
        : event_type(event_type), event_type_name(event_type_name) {
    }

    EventType Event::get_type() const {
        return event_type;
    }

    const std::string& Event::get_type_name() const {
        return event_type_name;
    }

    std::string Event::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{}";
        return ss.str();
    }
}

#include "st_event.h"

namespace Storytime {
    Event::Event(EventType type, const std::string& type_name)
        : event_type(type), event_type_name(type_name) {
    }

    std::string Event::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{}";
        return ss.str();
    }
}

#include "st_event.h"

namespace Storytime {
    Event::Event(EventType event_type, const std::string& event_type_name)
        : event_type(event_type), event_type_name(event_type_name) {
    }

    std::string Event::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{}";
        return ss.str();
    }
}

#include "st_window_closed_event.h"

namespace Storytime {
    const EventType WindowClosedEvent::type = 3557;
    const std::string WindowClosedEvent::type_name = "WindowClosedEvent";

    WindowClosedEvent::WindowClosedEvent() : Event(type, type_name) {
    }

    std::string WindowClosedEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const WindowClosedEvent& event) {
        return os << event.to_string();
    }
}

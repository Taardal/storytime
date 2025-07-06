#include "st_window_minimized_event.h"

namespace Storytime {
    const EventType WindowMinimizedEvent::type = 1063;
    const std::string WindowMinimizedEvent::type_name = "WindowMinimizedEvent";

    WindowMinimizedEvent::WindowMinimizedEvent() : Event(type, type_name) {
    }

    std::string WindowMinimizedEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{minimized=" << minimized << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const WindowMinimizedEvent& event) {
        return os << event.to_string();
    }
}

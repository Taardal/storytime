#include "st_window_focused_event.h"

namespace Storytime {
    const EventType WindowFocusedEvent::type = 1063;
    const std::string WindowFocusedEvent::type_name = "WindowFocusedEvent";

    WindowFocusedEvent::WindowFocusedEvent() : Event(type, type_name) {
    }

    std::string WindowFocusedEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{focused=" << focused << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const WindowFocusedEvent& event) {
        return os << event.to_string();
    }
}

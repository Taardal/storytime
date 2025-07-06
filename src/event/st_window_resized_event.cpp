#include "st_window_resized_event.h"

namespace Storytime {
    const EventType WindowResizedEvent::type = 1684;
    const std::string WindowResizedEvent::type_name = "WindowResizedEvent";

    WindowResizedEvent::WindowResizedEvent() : Event(type, type_name) {
    }

    std::string WindowResizedEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{width=" << width << ", height=" << height << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const WindowResizedEvent& event) {
        return os << event.to_string();
    }
}

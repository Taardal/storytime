#include "window_event.h"
#include <sstream>

namespace Storytime {
    const EventType WindowCloseEvent::type = 3557600239;
    const std::string WindowCloseEvent::name = "WindowCloseEvent";

    WindowCloseEvent::WindowCloseEvent()
        : Event(type, name) {
    }

    const EventType WindowResizeEvent::type = 3475249015;
    const std::string WindowResizeEvent::name = "WindowResizeEvent";

    WindowResizeEvent::WindowResizeEvent(i32 width, i32 height)
        : Event(type, name), width(width), height(height) {
    }

    std::string WindowResizeEvent::to_string() const {
        std::stringstream ss;
        ss << name << "{width=" << width << ", height=" << height << "}";
        return ss.str();
    }

    const EventType WindowMinimizeEvent::type = 1063552704;
    const std::string WindowMinimizeEvent::name = "WindowMinimizeEvent";

    WindowMinimizeEvent::WindowMinimizeEvent(bool minimized)
        : Event(type, name), minimized(minimized) {
    }

    std::string WindowMinimizeEvent::to_string() const {
        std::stringstream ss;
        ss << name << "{minimized=" << minimized << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const WindowMinimizeEvent& event) {
        os << event.to_string();
        return os;
    }
}

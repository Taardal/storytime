#include "window_event.h"
#include <sstream>

namespace Storytime {
    WindowCloseEvent::WindowCloseEvent()
        : Event(EventType::WindowClose, "WindowCloseEvent") {
    }

    WindowResizeEvent::WindowResizeEvent(i32 width, i32 height)
        : Event(EventType::WindowResize, "WindowResizeEvent"), width(width), height(height) {
    }

    std::string WindowResizeEvent::to_string() const {
        std::stringstream ss;
        ss << name << "{width=" << width << ", height=" << height << "}";
        return ss.str();
    }

    WindowMinimizeEvent::WindowMinimizeEvent(bool minimized)
        : Event(EventType::WindowMinimize, "WindowMinimizeEvent"), minimized(minimized) {
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

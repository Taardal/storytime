#include "WindowEvent.h"
#include <sstream>

namespace storytime {
    WindowCloseEvent::WindowCloseEvent()
            : Event("WindowCloseEvent", EventType::WindowClose) {
    }

    WindowResizeEvent::WindowResizeEvent(int width, int height)
            : Event("WindowResizeEvent", EventType::WindowResize), width(width), height(height) {
    }

    std::string WindowResizeEvent::toString() const {
        std::stringstream ss;
        ss << name << "{width=" << width << ", height=" << height << "}";
        return ss.str();
    }
}
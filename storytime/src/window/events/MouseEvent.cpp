#include "MouseEvent.h"
#include <sstream>

namespace storytime {
    MouseMovedEvent::MouseMovedEvent(float x, float y)
            : Event("MouseMovedEvent", EventType::MouseMoved), x(x), y(y) {
    }

    float MouseMovedEvent::getX() const {
        return x;
    }

    float MouseMovedEvent::getY() const {
        return y;
    }

    std::string MouseMovedEvent::toString() const {
        std::stringstream ss;
        ss << name << "{x=" << x << ", y=" << y << "}";
        return ss.str();
    }

    MouseScrolledEvent::MouseScrolledEvent(float xOffset, float yOffset)
            : Event("MouseScrolledEvent", EventType::MouseScrolled), xOffset(xOffset), yOffset(yOffset) {
    }

    float MouseScrolledEvent::getXOffset() const {
        return xOffset;
    }

    float MouseScrolledEvent::getYOffset() const {
        return yOffset;
    }

    std::string MouseScrolledEvent::toString() const {
        std::stringstream ss;
        ss << name << "{xOffset=" << xOffset << ", yOffset=" << yOffset << "}";
        return ss.str();
    }

    MouseButtonEvent::MouseButtonEvent(std::string_view name, EventType type, int button)
            : Event(name, type), button(button) {
    }

    std::string MouseButtonEvent::toString() const {
        std::stringstream ss;
        ss << name << "{button=" << button << "}";
        return ss.str();
    }

    int MouseButtonEvent::getButton() const {
        return button;
    }

    MouseButtonPressedEvent::MouseButtonPressedEvent(int button)
            : MouseButtonEvent("MouseButtonPressedEvent", EventType::MouseButtonPressed, button) {
    }

    MouseButtonReleasedEvent::MouseButtonReleasedEvent(int button)
            : MouseButtonEvent("MouseButtonReleasedEvent", EventType::MouseButtonReleased, button) {
    }
}

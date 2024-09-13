#include "event.h"

namespace Storytime {
    Event::Event(EventType type, const std::string& name) : type(type), name(name) {
    }

    std::string Event::get_name() const {
        return name;
    }

    EventType Event::get_type() const {
        return type;
    }

    std::string Event::to_string() const {
        std::stringstream ss;
        ss << name << "{}";
        return ss.str();
    }

    std::string Event::get_name(EventType event_type) {
        switch (event_type) {
            case EventType::WindowClose:
                return ST_STRING(WindowClose);
            case EventType::WindowResize:
                return ST_STRING(WindowResize);
            case EventType::KeyPressed:
                return ST_STRING(KeyPressed);
            case EventType::KeyReleased:
                return ST_STRING(KeyReleased);
            case EventType::KeyRepeated:
                return ST_STRING(KeyRepeated);
            case EventType::KeyTyped:
                return ST_STRING(KeyTyped);
            case EventType::MouseMoved:
                return ST_STRING(MouseMoved);
            case EventType::MouseScroll:
                return ST_STRING(MouseScrolled);
            case EventType::MouseButtonPressed:
                return ST_STRING(MouseButtonPressed);
            case EventType::MouseButtonReleased:
                return ST_STRING(MouseButtonReleased);
            default:
                return "";
        }
    }
}

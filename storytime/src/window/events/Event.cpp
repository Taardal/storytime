#include "Event.h"
#include <sstream>

namespace Storytime {
    Event::Event(std::string_view name, EventType type)
        : type(type), name(name), handled(false) {
    }

    std::string_view Event::GetName() const {
        return name;
    }

    EventType Event::GetType() const {
        return type;
    }

    bool Event::IsHandled() const {
        return handled;
    }

    void Event::SetHandled(bool handled) {
        this->handled = handled;
    }

    std::string Event::ToString() const {
        std::stringstream ss;
        ss << name << "{}";
        return ss.str();
    }

    std::string Event::get_name(EventType event_type) {
        switch (event_type) {
            case EventType::WindowClose:
                return ST_TO_STRING(WindowClose);
            case EventType::WindowResize:
                return ST_TO_STRING(WindowResize);
            case EventType::KeyPressed:
                return ST_TO_STRING(KeyPressed);
            case EventType::KeyReleased:
                return ST_TO_STRING(KeyReleased);
            case EventType::KeyRepeated:
                return ST_TO_STRING(KeyRepeated);
            case EventType::KeyTyped:
                return ST_TO_STRING(KeyTyped);
            case EventType::MouseMoved:
                return ST_TO_STRING(MouseMoved);
            case EventType::MouseScrolled:
                return ST_TO_STRING(MouseScrolled);
            case EventType::MouseButtonPressed:
                return ST_TO_STRING(MouseButtonPressed);
            case EventType::MouseButtonReleased:
                return ST_TO_STRING(MouseButtonReleased);
            default:
                return "";
        }
    }
}

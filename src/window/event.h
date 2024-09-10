#pragma once

#include <string>

namespace Storytime {
    enum class EventType {
        None = 0,
        WindowClose,
        WindowResize,
        WindowMinimize,
        KeyPressed,
        KeyReleased,
        KeyRepeated,
        KeyTyped,
        MouseMoved,
        MouseScroll,
        MouseButtonPressed,
        MouseButtonReleased
    };

    struct Event {
        EventType type;
        std::string name;

        Event(EventType type, const std::string& name);

        virtual ~Event() = default;

        virtual std::string to_string() const;

        std::string get_name() const;

        EventType get_type() const;

        static std::string get_name(EventType event_type);
    };
}

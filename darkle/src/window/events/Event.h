#pragma once

#include <string>

namespace Darkle {

    enum class EventType {
        None = 0,
        WindowClose,
        WindowResize,
        KeyPressed,
        KeyReleased,
        KeyRepeated,
        KeyTyped,
        MouseMoved,
        MouseScrolled,
        MouseButtonPressed,
        MouseButtonReleased
    };

    class Event {
    protected:
        std::string_view name;
        EventType type;
        bool handled;

    protected:
        Event(std::string_view name, EventType type);

        virtual ~Event() = default;

    public:
        [[nodiscard]] virtual std::string toString() const;

        [[nodiscard]] std::string_view getName() const;

        [[nodiscard]] EventType getType() const;

        [[nodiscard]] bool isHandled() const;

        void setHandled(bool handled);
    };

}



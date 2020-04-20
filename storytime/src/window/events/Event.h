#pragma once

#include <string>

namespace storytime
{
    enum class EventType
    {
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

    class Event
    {
    protected:
        std::string_view name;
        EventType type;
        bool handled;

    protected:
        Event(std::string_view name, EventType type);

        virtual ~Event() = default;

    public:
        [[nodiscard]] virtual std::string ToString() const;

        [[nodiscard]] std::string_view GetName() const;

        [[nodiscard]] EventType GetType() const;

        [[nodiscard]] bool IsHandled() const;

        void SetHandled(bool handled);
    };

}



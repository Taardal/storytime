#pragma once

#include <string>

namespace Storytime
{
    enum class EventType
    {
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

        static std::string get_name(EventType event_type);
    };

}



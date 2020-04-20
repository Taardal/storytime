#pragma once

#include "Event.h"

namespace storytime
{
    class KeyEvent : public Event
    {
    private:
        uint32_t keyCode;

    protected:
        KeyEvent(std::string_view name, EventType type, uint32_t keyCode);

    public:
        [[nodiscard]] std::string ToString() const override;

        [[nodiscard]] virtual uint32_t GetKeyCode() const;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        explicit KeyPressedEvent(unsigned keyCode);
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        explicit KeyReleasedEvent(unsigned keyCode);
    };

    class KeyRepeatedEvent : public KeyEvent
    {
    public:
        explicit KeyRepeatedEvent(unsigned keyCode);
    };

    class KeyTypedEvent : public KeyEvent
    {
    public:
        explicit KeyTypedEvent(unsigned keyCode);
    };

}



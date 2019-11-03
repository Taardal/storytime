#pragma once

#include "Event.h"

namespace Darkle {

    class KeyEvent : public Event {
    private:
        unsigned int keyCode;

    protected:
        KeyEvent(std::string_view name, EventType type, unsigned int keyCode);

    public:
        [[nodiscard]] std::string toString() const override;

        [[nodiscard]] virtual unsigned int getKeyCode() const;
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        explicit KeyPressedEvent(unsigned keyCode);
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        explicit KeyReleasedEvent(unsigned keyCode);
    };

    class KeyRepeatedEvent : public KeyEvent {
    public:
        explicit KeyRepeatedEvent(unsigned keyCode);
    };

    class KeyTypedEvent : public KeyEvent {
    public:
        explicit KeyTypedEvent(unsigned keyCode);
    };

}



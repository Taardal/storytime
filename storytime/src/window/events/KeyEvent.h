#pragma once

#include "Event.h"

namespace Storytime
{
    class KeyEvent : public Event
    {
    private:
        int32_t keyCode;
        int32_t mods;
        int32_t scanCode;

    protected:
        KeyEvent(std::string_view name, EventType type, int32_t keyCode);

    public:
        KeyEvent(const std::string_view& name, EventType type, int32_t key_code, int32_t mods, int32_t scan_code);

        [[nodiscard]] std::string ToString() const override;

        [[nodiscard]] virtual int32_t GetKeyCode() const;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        explicit KeyPressedEvent(int32_t key_code, int32_t mods, int32_t scan_code);
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        explicit KeyReleasedEvent(int32_t key_code, int32_t mods, int32_t scan_code);
    };

    class KeyRepeatedEvent : public KeyEvent
    {
    public:
        explicit KeyRepeatedEvent(int32_t key_code, int32_t mods, int32_t scan_code);
    };

    class KeyTypedEvent : public KeyEvent
    {
    public:
        explicit KeyTypedEvent(int32_t key_code, int32_t mods, int32_t scan_code);
    };

}



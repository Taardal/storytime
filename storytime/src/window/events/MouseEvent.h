#pragma once

#include "Event.h"

namespace storytime
{
    class MouseMovedEvent : public Event
    {
    private:
        float x;
        float y;

    public:
        MouseMovedEvent(float x, float y);

        std::string ToString() const override;

        float GetX() const;

        float GetY() const;
    };

    class MouseScrolledEvent : public Event
    {
    private:
        float xOffset;
        float yOffset;

    public:
        MouseScrolledEvent(float xOffset, float yOffset);

        std::string ToString() const override;

        float GetXOffset() const;

        float GetYOffset() const;
    };

    class MouseButtonEvent : public Event
    {
    private:
        int32_t button;

    protected:
        MouseButtonEvent(std::string_view name, EventType type, int32_t button);

        std::string ToString() const override;

    public:
        int32_t GetButton() const;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonPressedEvent(int32_t button);
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonReleasedEvent(int32_t button);
    };
}
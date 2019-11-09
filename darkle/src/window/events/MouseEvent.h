#pragma once

#include "Event.h"

namespace Darkle {
    class MouseMovedEvent : public Event {
    private:
        float x;
        float y;

    public:
        MouseMovedEvent(float x, float y);

        [[nodiscard]] std::string toString() const override;

        [[nodiscard]] float getX() const;

        [[nodiscard]] float getY() const;
    };

    class MouseScrolledEvent : public Event {
    private:
        float xOffset;
        float yOffset;

    public:
        MouseScrolledEvent(float xOffset, float yOffset);

        [[nodiscard]] std::string toString() const override;

        [[nodiscard]] float getXOffset() const;

        [[nodiscard]] float getYOffset() const;
    };

    class MouseButtonEvent : public Event {
    private:
        int button;

    protected:
        MouseButtonEvent(std::string_view name, EventType type, int button);

        [[nodiscard]] std::string toString() const override;

    public:
        [[nodiscard]] int getButton() const;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        explicit MouseButtonPressedEvent(int button);
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        explicit MouseButtonReleasedEvent(int button);
    };
}
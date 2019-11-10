#pragma once

#include "Event.h"

namespace storytime {
    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent();
    };

    class WindowResizeEvent : public Event {
    private:
        int width;
        int height;

    public:
        WindowResizeEvent(int width, int height);

        [[nodiscard]] int getWidth() const;

        [[nodiscard]] int getHeight() const;

        [[nodiscard]] std::string toString() const override;
    };
}
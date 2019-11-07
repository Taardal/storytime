#pragma once

#include "Event.h"

namespace Darkle {
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

        [[nodiscard]] std::string toString() const override;
    };
}
#pragma once

#include "Event.h"

namespace storytime
{
    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent();
    };

    class WindowResizeEvent : public Event
    {
    private:
        int32_t width;
        int32_t height;

    public:
        WindowResizeEvent(int32_t width, int32_t height);

        [[nodiscard]] int32_t GetWidth() const;

        [[nodiscard]] int32_t GetHeight() const;

        [[nodiscard]] std::string ToString() const override;
    };
}
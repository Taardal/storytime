#include "WindowEvent.h"
#include <sstream>

namespace storytime
{
    WindowCloseEvent::WindowCloseEvent()
            : Event("WindowCloseEvent", EventType::WindowClose)
    {
    }

    WindowResizeEvent::WindowResizeEvent(int32_t width, int32_t height)
            : Event("WindowResizeEvent", EventType::WindowResize), width(width), height(height)
    {
    }

    int32_t WindowResizeEvent::GetWidth() const
    {
        return width;
    }

    int32_t WindowResizeEvent::GetHeight() const
    {
        return height;
    }

    std::string WindowResizeEvent::ToString() const
    {
        std::stringstream ss;
        ss << name << "{width=" << width << ", height=" << height << "}";
        return ss.str();
    }
}
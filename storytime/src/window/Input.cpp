#include "system/log.h"
#include "Input.h"
#include <GLFW/glfw3.h>

namespace Storytime
{
    bool Input::IsKeyPressed(int32_t keyCode) const
    {
        int32_t state = glfwGetKey(glfwGetCurrentContext(), keyCode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(int32_t keyCode) const
    {
        int32_t state = glfwGetMouseButton(glfwGetCurrentContext(), keyCode);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::GetMousePosition() const
    {
        double x;
        double y;
        glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
        return { (float) x, (float) y };
    }

}


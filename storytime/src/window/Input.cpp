#include "system/Log.h"
#include "Input.h"
#include <GLFW/glfw3.h>

namespace storytime {

    Input::Input() {
        ST_LOG_TRACE(ST_TAG, "Created");
    }

    Input::~Input() {
        ST_LOG_TRACE(ST_TAG, "Destroyed");
    }

    bool Input::isKeyPressed(int keyCode) const {
        int state = glfwGetKey(glfwGetCurrentContext(), keyCode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::isMouseButtonPressed(int keyCode) const {
        int state = glfwGetMouseButton(glfwGetCurrentContext(), keyCode);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::getMousePosition() const {
        double x;
        double y;
        glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
        return { (float) x, (float) y };
    }

}


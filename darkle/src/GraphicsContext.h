#pragma once

#include <GLFW/glfw3.h>

namespace Darkle {

    class GraphicsContext {
    public:
        GraphicsContext();

        ~GraphicsContext();

        void init(GLFWwindow* glfwWindow) const;

    private:
        void initGlad() const;

        void logContext() const;
    };

}



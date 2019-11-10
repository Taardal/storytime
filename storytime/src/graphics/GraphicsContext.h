#pragma once

#include <GLFW/glfw3.h>

namespace storytime {

    class GraphicsContext {
    public:
        struct Config {
            int openGLVersionMajor;
            int openGLVersionMinor;
        };

    private:
        const Config& config;

    public:
        explicit GraphicsContext(const Config& config);

        ~GraphicsContext();

        [[nodiscard]] const Config& getConfig() const;

        void init(GLFWwindow* glfwWindow) const;

    private:
        void initGlad() const;

        void logContext() const;
    };

}



#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace storytime
{
    class GraphicsContext
    {
    public:
        struct Config
        {
            uint32_t OpenGLVersionMajor = 4;
            uint32_t OpenGLVersionMinor = 1;
            const char* GLSLVersion = "#version 410";
        };

    private:
        const Config& config;

    public:
        explicit GraphicsContext(const Config& config);

        const Config& getConfig() const;

        void Init(GLFWwindow* glfwWindow) const;

    private:
        void InitGlad() const;

        void LogContext() const;
    };

}



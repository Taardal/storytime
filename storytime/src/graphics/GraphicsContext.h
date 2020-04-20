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
            uint32_t versionMajor;
            uint32_t versionMinor;
        };

    private:
        const Config& config;

    public:
        explicit GraphicsContext(const Config& config);

        ~GraphicsContext();

        [[nodiscard]] const Config& getConfig() const;

        void Init(GLFWwindow* glfwWindow) const;

    private:
        void InitGlad() const;

        void LogContext() const;
    };

}



#pragma once

#include "window/new_window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Storytime {
    class GraphicsContext {
    public:
        struct Config {
            NewWindow* window = nullptr;
            uint32_t VersionMajor;
            uint32_t VersionMinor;
            const char* GlslVersion;
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

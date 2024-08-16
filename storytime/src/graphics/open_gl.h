#pragma once

#include "window/window.h"

namespace Storytime {
    struct OpenGLConfig {
        Window* window = nullptr;
        LogLevel log_level;
        u32 major_version;
        u32 minor_version;
        std::string glsl_version;
    };

    class OpenGL {
    private:
        const OpenGLConfig& config;

    public:
        explicit OpenGL(const OpenGLConfig& config);
    };
}

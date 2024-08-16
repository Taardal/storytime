#pragma once

#include "system/system_module.h"
#include "window/window_module.h"
#include "graphics/open_gl.h"
#include "graphics/imgui_renderer.h"
#include "graphics/renderer.h"
#include "graphics/orthographic_camera.h"

namespace Storytime {
    struct GraphicsModuleConfig {
        SystemModule* system_module = nullptr;
        WindowModule* window_module = nullptr;
        OpenGLConfig open_gl_config;
    };

    struct GraphicsModule {
        OpenGL open_gl;
        Renderer renderer;
        ImGuiRenderer imgui_renderer;
        OrthographicCamera camera;

        explicit GraphicsModule(const GraphicsModuleConfig& config);
    };
}

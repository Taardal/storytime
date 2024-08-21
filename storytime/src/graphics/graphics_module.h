#pragma once

#include "storytime_config.h"
#include "system/system_module.h"
#include "window/window_module.h"
#include "graphics/open_gl.h"
#include "graphics/imgui_renderer.h"
#include "graphics/renderer.h"
#include "graphics/camera.h"

namespace Storytime {
    struct GraphicsModule {
        OpenGL open_gl;
        Renderer renderer;
        ImGuiRenderer imgui_renderer;
        Camera camera;

        GraphicsModule(const Config& config, SystemModule* system_module, WindowModule* window_module);
    };
}

#pragma once

#include "storytime_config.h"
#include "system/system_module.h"
#include "window/window_module.h"
#include "graphics/imgui_renderer.h"
#include "graphics/renderer.h"
#include "graphics/camera.h"

namespace Storytime {
    struct GraphicsModule {
        Renderer renderer;
        ImGuiRenderer imgui_renderer;
        Camera camera;

        GraphicsModule(const Config& config, SystemModule* system_module, WindowModule* window_module);

        static void initialize(const Config& config);
    };
}

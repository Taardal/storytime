#pragma once

#include "storytime_config.h"
#include "graphics/renderer.h"
#include "graphics/imgui_renderer.h"
#include "graphics/camera.h"
#include "window/window_module.h"
#include "resource/resource_module.h"

namespace Storytime {
    class GraphicsModule : public Module {
    private:
        Renderer renderer;
        ImGuiRenderer imgui_renderer;
        Camera camera;

    public:
        GraphicsModule(
            const Config& config,
            WindowModule* window_module,
            ResourceModule* resource_module
        );

        static void initialize(const Config& config);

        static void terminate();
    };
}

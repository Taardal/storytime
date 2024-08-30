#pragma once

#include "storytime_app.h"
#include "storytime_config.h"
#include "window/window.h"
#include "graphics/renderer.h"
#include "graphics/imgui_renderer.h"
#include "graphics/camera.h"
#include "graphics/graphics_module.h"
#include "window/window_module.h"

namespace Storytime {
    class Engine {
    private:
        bool running = false;
        Config config;
        Window* window;
        Camera* camera;
        Renderer* renderer;
        ImGuiRenderer* imgui_renderer;

    public:
        Engine(
            const Config& config,
            WindowModule* window_module,
            GraphicsModule* graphics_module
        );

        void run(App* app);

        void stop();
    };
}
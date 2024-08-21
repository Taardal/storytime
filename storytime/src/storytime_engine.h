#pragma once

#include "storytime_app.h"
#include "system/service_locator.h"
#include "window/window.h"
#include "window/event_manager.h"
#include "graphics/renderer.h"
#include "graphics/imgui_renderer.h"
#include "graphics/camera.h"

namespace Storytime {
    struct EngineConfig {
        ServiceLocator* service_locator = nullptr;
        Window* window = nullptr;
        EventManager* event_manager = nullptr;
        Renderer* renderer = nullptr;
        ImGuiRenderer* imgui_renderer = nullptr;
        Camera* camera = nullptr;
        u32 target_fps = 60;
    };

    class Engine {
    private:
        EngineConfig config;
        bool running = false;

    public:
        explicit Engine(const EngineConfig& config);

        virtual ~Engine() = default;

        void run(App* app);

        void stop();
    };
}

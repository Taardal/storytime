#pragma once

#include "storytime_config.h"
#include "graphics/renderer.h"
#include "graphics/imgui_renderer.h"
#include "graphics/camera.h"
#include "system/system_module.h"
#include "window/window_module.h"
#include "resource/resource_module.h"

namespace Storytime {
    struct GraphicsModule {
        SystemModule* system_module;
        Renderer renderer;
        ImGuiRenderer imgui_renderer;
        Camera camera;

        GraphicsModule(
            const Config& config,
            SystemModule* system_module,
            WindowModule* window_module,
            ResourceModule* resource_module
        );

        static void initialize(const Config& config);

        template<typename T>
        T* get() {
            return system_module->service_locator.get<T>();
        }
    };
}

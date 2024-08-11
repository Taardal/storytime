#pragma once

#include "system/system_module.h"
#include "graphics/GraphicsContext.h"
#include "graphics/ImGuiRenderer.h"
#include "graphics/Renderer.h"
#include "graphics/OrthographicCameraController.h"

namespace Storytime {
    struct GraphicsModuleConfig {
        SystemModule* system_module = nullptr;
        GraphicsContext::Config context_config;
    };

    struct GraphicsModule {
        GraphicsContext graphics_context;
        ImGuiRenderer im_gui_renderer;
        Renderer renderer;
        OrthographicCamera camera;
        OrthographicCameraController camera_controller;

        explicit GraphicsModule(const GraphicsModuleConfig& config);
    };
}

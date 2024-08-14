#include "graphics_module.h"

namespace Storytime {
    GraphicsModule::GraphicsModule(const GraphicsModuleConfig& config)
        : graphics_context(config.context_config),
          imgui_renderer(&graphics_context, &config.window_module->window),
          renderer(&config.system_module->resource_loader),
          camera(),
          camera_controller(&camera, 16.0f / 9.0f) {
    }
}

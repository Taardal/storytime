#include "graphics_module.h"

namespace Storytime {
    GraphicsModule::GraphicsModule(const GraphicsModuleConfig& config)
        : graphics_context(config.context_config),
          im_gui_renderer(&graphics_context),
          renderer(&config.system_module->resourceLoader),
          camera(),
          camera_controller(&camera, 16.0f / 9.0f) {
    }
}

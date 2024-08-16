#include "graphics_module.h"

namespace Storytime {
    GraphicsModule::GraphicsModule(const GraphicsModuleConfig& config)
        : open_gl(config.open_gl_config),
          renderer(&config.system_module->resource_loader),
          imgui_renderer({
              .window = &config.window_module->window,
              .glsl_version = config.open_gl_config.glsl_version,
          }),
          camera()
    {
        config.system_module->service_locator.set<Renderer>(&renderer);
        config.system_module->service_locator.set<ImGuiRenderer>(&imgui_renderer);
        config.system_module->service_locator.set<OrthographicCamera>(&camera);
    }
}

#include "graphics_module.h"

#include "open_gl.h"

namespace Storytime {
    GraphicsModule::GraphicsModule(const Config& config, SystemModule* system_module, WindowModule* window_module)
        : renderer(&system_module->resource_loader),
          imgui_renderer({
              .window = &window_module->window,
              .glsl_version = config.glsl_version,
          }),
          camera()
    {
        system_module->service_locator.set<Renderer>(&renderer);
        system_module->service_locator.set<ImGuiRenderer>(&imgui_renderer);
        system_module->service_locator.set<Camera>(&camera);
    }

    void GraphicsModule::initialize(const Config& config) {
        OpenGL::initialize({
            .log_level = config.log_level,
            .major_version = config.open_gl_version_major,
            .minor_version = config.open_gl_version_minor,
            .glsl_version = config.glsl_version,
        });
    }

    void GraphicsModule::terminate() {
    }
}

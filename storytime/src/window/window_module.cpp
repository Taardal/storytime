#include "window_module.h"

namespace Storytime {
    WindowModule::WindowModule(const Config& config, SystemModule* system_module)
        : event_manager(),
          window({
              .event_manager = &event_manager,
              .title = config.window_title,
              .width = config.window_width,
              .height = config.window_height,
              .maximized = config.window_maximized,
              .resizable = config.window_resizable,
              .context_version_major = config.open_gl_version_major,
              .context_version_minor = config.open_gl_version_minor,
          }),
          user_input()
    {
        system_module->service_locator.set<EventManager>(&event_manager);
        system_module->service_locator.set<Window>(&window);
        system_module->service_locator.set<UserInput>(&user_input);
    }
}

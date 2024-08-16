#include "window_module.h"

namespace Storytime {
    WindowModule::WindowModule(const WindowModuleConfig& config)
        : config(config),
          event_manager(),
          window(config.window_config, &event_manager),
          user_input()
    {
        config.system_module->service_locator.set<EventManager>(&event_manager);
        config.system_module->service_locator.set<Window>(&window);
        config.system_module->service_locator.set<UserInput>(&user_input);
    }
}

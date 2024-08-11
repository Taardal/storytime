#include "window_module.h"

namespace Storytime {
    WindowModule::WindowModule(const WindowModuleConfig& config)
        : config(config),
          event_manager(),
          window(config.window_config, &event_manager),
          input() {
    }
}

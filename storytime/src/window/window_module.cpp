#include "window_module.h"

namespace Storytime {
    WindowModule::WindowModule(const WindowModuleConfig& config)
        : config(config),
          window(config.window_config) {
    }
}

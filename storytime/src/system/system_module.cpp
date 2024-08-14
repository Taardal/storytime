#include "system_module.h"

namespace Storytime {
    SystemModule::SystemModule()
        : service_locator(),
          file_system(),
          resource_loader(&file_system) {
    }
}

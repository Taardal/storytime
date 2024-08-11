#include "system_module.h"

namespace Storytime {
    SystemModule::SystemModule() : fileSystem(), resourceLoader(&fileSystem) {
    }
}

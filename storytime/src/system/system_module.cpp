#include "system_module.h"

namespace Storytime {
    SystemModule::SystemModule()
        : service_locator(),
          file_system()
    {
        service_locator.set<ServiceLocator>(&service_locator);
        service_locator.set<FileSystem>(&file_system);
    }

    void SystemModule::initialize(const Config& config) {
        set_log_level(config.log_level);
        initialize_error_signal_handlers();
#ifdef ST_TRACK_MEMORY
        std::atexit(MemoryTracker::terminate);
#endif
    }
}

#pragma once

#include "storytime_config.h"
#include "service_locator.h"
#include "file_system.h"
#include "resource_loader.h"

namespace Storytime {
    struct SystemModule {
        ServiceLocator service_locator;
        FileSystem file_system;
        ResourceLoader resource_loader;

        SystemModule();

        static void initialize(const Config& config);
    };
}

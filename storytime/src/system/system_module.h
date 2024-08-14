#pragma once

#include "FileSystem.h"
#include "ResourceLoader.h"
#include "service_locator.h"

namespace Storytime {
    struct SystemModule {
        ServiceLocator service_locator;
        FileSystem file_system;
        ResourceLoader resource_loader;

        SystemModule();
    };
}

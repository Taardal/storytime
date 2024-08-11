#pragma once

#include "FileSystem.h"
#include "ResourceLoader.h"

namespace Storytime {
    struct SystemModule {
        FileSystem fileSystem;
        ResourceLoader resourceLoader;

        SystemModule();
    };
}

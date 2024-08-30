#pragma once

#include "resource/resource_loader.h"
#include "audio/audio_module.h"
#include "system/system_module.h"

namespace Storytime {
    struct ResourceModule {
        ResourceLoader resource_loader;

        explicit ResourceModule(SystemModule* system_module, AudioModule* audio_module);
    };
}

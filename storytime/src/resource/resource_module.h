#pragma once

#include "resource/resource_loader.h"
#include "audio/audio_module.h"
#include "system/system_module.h"

namespace Storytime {
    class ResourceModule : public Module {
    private:
        ResourceLoader resource_loader;

    public:
        explicit ResourceModule(SystemModule* system_module, AudioModule* audio_module);

        static void initialize();

        static void terminate();
    };
}

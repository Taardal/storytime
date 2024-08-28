#pragma once

#include "resource/resource_loader.h"
#include "audio/audio_module.h"
#include "system/system_module.h"

namespace Storytime {
    struct ResourceModule {
<<<<<<< HEAD
        SystemModule* system_module;
        ResourceLoader resource_loader;

        explicit ResourceModule(SystemModule* system_module, AudioModule* audio_module);

        template<typename T>
        T* get() {
            return system_module->service_locator.get<T>();
        }
=======
        ResourceLoader resource_loader;

        explicit ResourceModule(SystemModule* system_module, AudioModule* audio_module);
>>>>>>> 3071683 (Audio module)
    };
}

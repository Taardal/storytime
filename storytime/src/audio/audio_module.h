#pragma once

#include "audio/audio_engine.h"
#include "system/system_module.h"

namespace Storytime {
    struct AudioModule {
        SystemModule* system_module;
        AudioEngine audio_engine;

        explicit AudioModule(SystemModule* system_module);

        template<typename T>
        T* get() {
            return system_module->service_locator.get<T>();
        }
    };
}

#pragma once

#include "audio/audio_engine.h"
#include "system/system_module.h"

namespace Storytime {
    struct AudioModule {
        AudioEngine audio_engine;

        explicit AudioModule(SystemModule* system_module);
    };
}

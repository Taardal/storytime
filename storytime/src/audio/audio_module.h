#pragma once

#include "audio/audio_engine.h"
#include "system/system_module.h"

namespace Storytime {
    class AudioModule : public SystemModule {
    private:
        AudioEngine audio_engine;

    public:
        explicit AudioModule();

        static void initialize();
    };
}

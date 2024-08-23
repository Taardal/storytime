#pragma once

#include "audio/audio_player.h"
#include "system/system_module.h"

namespace Storytime {
    struct AudioModule {
        AudioPlayer audio_player;

        AudioModule(SystemModule* system_module);
    };
}

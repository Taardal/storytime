#include "audio_module.h"

namespace Storytime {
    AudioModule::AudioModule(SystemModule* system_module) {
        system_module->service_locator.set<AudioPlayer>(&audio_player);
    }
}

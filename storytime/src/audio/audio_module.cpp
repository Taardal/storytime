#include "audio_module.h"

namespace Storytime {
    AudioModule::AudioModule() {
        service_locator.set<AudioEngine>(&audio_engine);
    }

    void AudioModule::initialize() {
    }
}

#include "audio_engine.h"

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace Storytime {
    AudioEngine::AudioEngine() {
        ST_LOG_TRACE("Initializing miniaudio");
        ma_engine_config* ma_config = nullptr;
        if (ma_engine_init(ma_config, &engine) != MA_SUCCESS) {
            ST_THROW("Could not initialize miniaudio");
        }
        ST_LOG_INFO("Initialized miniaudio");
    }

    AudioEngine::~AudioEngine() {
        ma_engine_uninit(&engine);
        ST_LOG_INFO("Terminated miniaudio");
    }

    AudioEngine::operator ma_engine*() {
        return &engine;
    }

    void AudioEngine::play(const char* path) {
        ma_sound_group* sound_group = nullptr;
        ma_engine_play_sound(&engine, path, sound_group);
    }

    void AudioEngine::play(const std::string& path) {
        play(path.c_str());
    }

    void AudioEngine::stop() {
        ma_engine_stop(&engine);
    }

    u32 AudioEngine::get_volume() {
        return static_cast<u32>(ma_engine_get_volume(&engine));
    }

    void AudioEngine::set_volume(u32 volume_percent) {
        ma_engine_set_volume(&engine, static_cast<f32>(volume_percent) / 100.0f);
    }
}

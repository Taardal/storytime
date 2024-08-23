#include "audio_player.h"

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace Storytime {
    AudioPlayer::AudioPlayer() {
        ST_LOG_TRACE("Initializing miniaudio");
        ma_engine_config* ma_config = nullptr;
        if (ma_engine_init(ma_config, &engine) != MA_SUCCESS) {
            ST_THROW("Could not initialize miniaudio");
        }
        ST_LOG_INFO("Initialized miniaudio");
    }

    AudioPlayer::~AudioPlayer() {
        ma_engine_uninit(&engine);
        ST_LOG_INFO("Terminated miniaudio");
    }

    void AudioPlayer::play(const char* path) {
        ST_LOG_TRACE("Playing sound [{}]", path);
        ma_sound_group* sound_group = nullptr;
        ma_engine_play_sound(&engine, path, sound_group);
    }

    void AudioPlayer::play(const std::string& path) {
        play(path.c_str());
    }

    void AudioPlayer::stop() {
        ma_engine_stop(&engine);
    }
}

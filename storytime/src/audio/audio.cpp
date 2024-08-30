#include "audio.h"

namespace Storytime {
    Audio::Audio(AudioEngine* engine, const std::string& path) {
        ST_ASSERT_THROW(engine != nullptr);
        ma_uint32 flags = 0;
        ma_sound_group* sound_group = nullptr;
        ma_fence* done_fence = nullptr;
        ma_result result = ma_sound_init_from_file(*engine, path.c_str(), flags, sound_group, done_fence, &sound);
        if (result != MA_SUCCESS) {
            ST_THROW("Could not load sound [" + std::string(path) + "]");
        }
    }

    Audio::~Audio() {
        ma_sound_uninit(&sound);
    }

    void Audio::play() {
        ma_sound_start(&sound);
    }

    void Audio::pause() {
        ma_sound_stop(&sound);
    }

    void Audio::stop() {
        ma_sound_stop(&sound);
        ma_sound_seek_to_pcm_frame(&sound, 0); // Rewind sound to beginning
    }

    u32 Audio::get_volume() const {
        return static_cast<u32>(ma_sound_get_volume(&sound));
    }

    void Audio::set_volume(u32 volume_percent) {
        ma_sound_set_volume(&sound, static_cast<f32>(volume_percent) / 100.0f);
    }

    bool Audio::is_playing() const {
        return ma_sound_is_playing(&sound);
    }
}

#pragma once

#include "audio/st_audio_engine.h"
#include <miniaudio.h>

namespace Storytime {
    class Audio {
    private:
        ma_sound sound;

    public:
        Audio(AudioEngine* engine, const std::string& path);

        ~Audio();

        void play();

        void pause();

        void stop();

        // Returns the volume in percentage from 0% (completely silent) to 100% (full volume) or more
        u32 get_volume() const;

        // Set the volume from 0% (completely silent) to 100% (full volume).
        // Values greater than 100% are also allowed and can amplify the sound beyond its original level.
        void set_volume(u32 volume_percent);

        bool is_playing() const;
    };
}

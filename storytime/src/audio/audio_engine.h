#pragma once

#include <miniaudio.h>

namespace Storytime {
    class AudioEngine {
    private:
        ma_engine engine;

    public:
        AudioEngine();

        ~AudioEngine();

        operator ma_engine*();

        void play(const char* path);

        void play(const std::string& path);

        void stop();

        // Returns the volume in percentage from 0% (completely silent) to 100% (full volume) or more
        u32 get_volume();

        // Set the volume from 0% (completely silent) to 100% (full volume).
        // Values greater than 100% are also allowed and can amplify the sound beyond its original level.
        void set_volume(u32 volume_percent);
    };
}

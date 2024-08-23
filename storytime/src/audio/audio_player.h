#pragma once

#include <miniaudio.h>

namespace Storytime {
    class AudioPlayer {
    private:
        ma_engine engine;

    public:
        AudioPlayer();

        ~AudioPlayer();

        void play(const char* path);

        void play(const std::string& path);

        void stop();
    };
}

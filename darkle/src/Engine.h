#pragma once

#include <sstream>
#include "Log.h"
#include "Application.h"

namespace Darkle {

    struct Config {
        LogLevel logLevel;
        std::string name;
    };

    class Engine {
    private:
        Application* application;

    public:
        explicit Engine(const Config& config);

        ~Engine();

        void run();
    };

}



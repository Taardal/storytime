#pragma once

#include "Log.h"
#include "Application.h"
#include <sstream>

namespace Darkle {

    class Engine {
    public:
        struct Config {
            LogLevel logLevel;
            Window::Config windowConfig;
        };

    private:
        GraphicsContext* graphicsContext;
        Window* window;
        Application* application;

    public:
        explicit Engine(const Config& config);

        ~Engine();

        void run();
    };

}



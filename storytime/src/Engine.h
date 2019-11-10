#pragma once

#include "system/Log.h"
#include "system/Application.h"

namespace storytime {

    class Engine {
    public:
        struct Config {
            LogLevel logLevel;
            Window::Config windowConfig;
            GraphicsContext::Config graphicsConfig;
        };

    private:
        GraphicsContext* graphicsContext;
        Window* window;
        Renderer* renderer;
        Input* input;
        Application* application;

    public:
        explicit Engine(const Config& config);

        ~Engine();

        void run();
    };

}



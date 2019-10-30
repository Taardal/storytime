#include "Engine.h"

namespace Darkle {

    Engine::Engine(const Config& config) {
        Log::setLevel(config.logLevel);
        graphicsContext = new GraphicsContext();
        window = new Window(config.windowConfig, graphicsContext);
        application = new Application(window);
        LOG_TRACE(TAG, "Created");
    }

    Engine::~Engine() {
        delete graphicsContext;
        delete window;
        delete application;
        LOG_TRACE(TAG, "Destroyed");
    }

    void Engine::run() {
        application->run();
    }


}
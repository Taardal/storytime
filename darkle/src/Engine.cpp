#include "Engine.h"

namespace Darkle {

    Engine::Engine(const Config& config) {
        Log::setLevel(config.logLevel);
        application = new Application();
        LOG_TRACE(TAG, "Created");
    }

    Engine::~Engine() {
        delete application;
        LOG_TRACE(TAG, "Destroyed");
    }

    void Engine::run() {
        application->run();
    }


}
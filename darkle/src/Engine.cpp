#include "Engine.h"

namespace Darkle {

    Engine::Engine(const Config& config) {
        Log::setLevel(config.logLevel);
        LOG_TRACE(TAG, "Creating");
        graphicsContext = new GraphicsContext(config.graphicsConfig);
        window = new Window(config.windowConfig, graphicsContext);
        renderer = new Renderer();
        application = new Application(window, renderer);
        LOG_TRACE(TAG, "Created");
    }

    Engine::~Engine() {
        LOG_TRACE(TAG, "Destroying");
        delete renderer;
        delete window;
        delete graphicsContext;
        delete application;
        LOG_TRACE(TAG, "Destroyed");
    }

    void Engine::pushLayer(Layer* layer) {
        application->pushLayer(layer);
    }

    void Engine::run() {
        application->run();
    }

}
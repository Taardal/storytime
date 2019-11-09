#include "Engine.h"

extern storytime::Application* CreateApplication(storytime::Window* window, storytime::Renderer* renderer);

namespace storytime {

    Engine::Engine(const Config& config) {
        Log::setLevel(config.logLevel);
        ST_TRACE(ST_TAG, "Creating");
        graphicsContext = new GraphicsContext(config.graphicsConfig);
        window = new Window(config.windowConfig, graphicsContext);
        renderer = new Renderer();
        application = CreateApplication(window, renderer);
        ST_TRACE(ST_TAG, "Created");
    }

    Engine::~Engine() {
        ST_TRACE(ST_TAG, "Destroying");
        delete application;
        delete renderer;
        delete window;
        delete graphicsContext;
        ST_TRACE(ST_TAG, "Destroyed");
    }

    void Engine::run() {
        application->run();
    }

}
#include "Engine.h"

extern storytime::Application* CreateApplication(storytime::Window* window, storytime::Renderer* renderer, storytime::Input* input);

namespace storytime {

    Engine::Engine(const Config& config) {
        Log::setLevel(config.logLevel);
        ST_TRACE(ST_TAG, "Creating");
        graphicsContext = new GraphicsContext(config.graphicsConfig);
        window = new Window(config.windowConfig, graphicsContext);
        renderer = new Renderer();
        input = new Input();
        application = CreateApplication(window, renderer, input);
        ST_TRACE(ST_TAG, "Created");
    }

    Engine::~Engine() {
        ST_TRACE(ST_TAG, "Destroying");
        delete application;
        delete input;
        delete renderer;
        delete window;
        delete graphicsContext;
        ST_TRACE(ST_TAG, "Destroyed");
    }

    void Engine::run() {
        application->run();
    }

}
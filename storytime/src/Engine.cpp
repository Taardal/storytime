#include "Engine.h"

extern storytime::Application* CreateApplication(
        storytime::Window* window,
        storytime::Renderer* renderer,
        storytime::Input* input,
        storytime::OrthographicCameraController* cameraController
);

namespace storytime
{
    Engine::Engine(const Config& config)
    {
        Log::setLevel(config.logLevel);
        ST_TRACE(ST_TAG, "Creating");
        fileSystem = new FileSystem();
        resourceLoader = new ResourceLoader(fileSystem);
        graphicsContext = new GraphicsContext(config.graphicsConfig);
        window = new Window(config.windowConfig, graphicsContext);
        renderer = new Renderer(resourceLoader);
        input = new Input();
        camera = new OrthographicCamera();
        cameraController = new OrthographicCameraController(camera, config.windowConfig.getAspectRatio());
        application = CreateApplication(window, renderer, input, cameraController);
        ST_TRACE(ST_TAG, "Created");
    }

    Engine::~Engine()
    {
        ST_TRACE(ST_TAG, "Destroying");
        delete application;
        delete cameraController;
        delete camera;
        delete input;
        delete renderer;
        delete window;
        delete graphicsContext;
        delete resourceLoader;
        delete fileSystem;
        ST_TRACE(ST_TAG, "Destroyed");
    }

    void Engine::run()
    {
        application->run();
    }

}
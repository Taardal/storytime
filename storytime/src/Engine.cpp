#include "Engine.h"

extern storytime::Application* CreateApplication(
        storytime::Window* window,
        storytime::Renderer* renderer,
        storytime::Input* input,
        storytime::OrthographicCameraController* cameraController,
        storytime::ResourceLoader* resourceLoader
);

namespace storytime
{
    Engine::Engine(const Config& config)
    {
        Log::Init(config.logLevel);

        ST_LOG_TRACE(ST_TAG, "Creating");

        fileSystem = new FileSystem();
        resourceLoader = new ResourceLoader(fileSystem);

        graphicsContext = new GraphicsContext(config.graphicsConfig);
        window = new Window(config.windowConfig, graphicsContext);
        GraphicsLog::Init(graphicsContext);
        renderer = new Renderer(resourceLoader);

        input = new Input();
        camera = new OrthographicCamera();
        cameraController = new OrthographicCameraController(camera, config.windowConfig.getAspectRatio());

        application = CreateApplication(window, renderer, input, cameraController, resourceLoader);

        ST_LOG_TRACE(ST_TAG, "Created");
    }

    Engine::~Engine()
    {
        ST_LOG_TRACE(ST_TAG, "Destroying");
        delete application;
        delete cameraController;
        delete camera;
        delete input;
        delete renderer;
        delete window;
        delete graphicsContext;
        delete resourceLoader;
        delete fileSystem;
        ST_LOG_TRACE(ST_TAG, "Destroyed");
    }

    void Engine::Run() const
    {
        application->run();
    }

}
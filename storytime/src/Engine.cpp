#include "Engine.h"

extern storytime::Application* CreateApplication(
        storytime::Window* window,
        storytime::Renderer* renderer,
        storytime::ImGuiRenderer* imGuiRenderer,
        storytime::Input* input,
        storytime::OrthographicCameraController* cameraController,
        storytime::ResourceLoader* resourceLoader
);

namespace storytime
{
    Engine::Engine(const Config& config)
    {
        Log::Init(config.logLevel);

        fileSystem = new FileSystem();
        resourceLoader = new ResourceLoader(fileSystem);

        graphicsContext = new GraphicsContext(config.graphicsConfig);
        imGuiRenderer = new ImGuiRenderer(graphicsContext);
        window = new Window(config.windowConfig, graphicsContext, imGuiRenderer);
        GraphicsLog::Init(graphicsContext);
        renderer = new Renderer(resourceLoader);

        input = new Input();
        camera = new OrthographicCamera();
        cameraController = new OrthographicCameraController(camera, config.windowConfig.GetAspectRatio());

        application = CreateApplication(window, renderer, imGuiRenderer, input, cameraController, resourceLoader);

        ST_LOG_TRACE(ST_TAG, "Created");
    }

    Engine::~Engine()
    {
        delete application;
        delete cameraController;
        delete camera;
        delete input;
        delete renderer;
        delete imGuiRenderer;
        delete window;
        delete graphicsContext;
        delete resourceLoader;
        delete fileSystem;
        ST_LOG_TRACE(ST_TAG, "Destroyed");
    }

    void Engine::Run() const
    {
        application->Run();
    }

}
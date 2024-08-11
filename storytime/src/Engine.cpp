#include "Engine.h"
#include "graphics/GraphicsLog.h"

extern Storytime::Application* CreateApplication(
        Storytime::Window* window,
        Storytime::Renderer* renderer,
        Storytime::ImGuiRenderer* imGuiRenderer,
        Storytime::Input* input,
        Storytime::OrthographicCameraController* cameraController,
        Storytime::ResourceLoader* resourceLoader
);

namespace Storytime
{
    Engine::Engine(const Config& config)
    {
        set_log_level(config.logLevel);
        GraphicsLog::Init(config.graphicsConfig);

        fileSystem = new FileSystem();
        resourceLoader = new ResourceLoader(fileSystem);

        graphicsContext = new GraphicsContext(config.graphicsConfig);
        imGuiRenderer = new ImGuiRenderer(graphicsContext);
        window = new Window(config.windowConfig, graphicsContext, imGuiRenderer);
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
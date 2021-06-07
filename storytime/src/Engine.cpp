#include "Engine.h"
#include "Game.h"

extern storytime::Game* CreateGame(
        storytime::CameraController* cameraController,
        storytime::Input* input,
        storytime::Window* window,
        storytime::Renderer* renderer,
        storytime::ResourceLoader* resourceLoader
);

namespace storytime
{
    Engine::Engine(const Config& config)
    {
        Log::Init(config.LogLevel);
        GraphicsLog::Init(config.Graphics);

        fileSystem = new FileSystem();
        resourceLoader = new ResourceLoader(fileSystem);

        graphicsContext = new GraphicsContext(config.Graphics);
        imGuiRenderer = new ImGuiRenderer(graphicsContext);
        window = new Window(config.Window, graphicsContext, imGuiRenderer);
        renderer = new Renderer(resourceLoader);

        input = new Input();
        camera = new Camera();
        cameraController = new CameraController(camera, input, config.Window.GetAspectRatio());

        game = CreateGame(cameraController, input, window, renderer, resourceLoader);
        application = new Application(game, camera, window, renderer, imGuiRenderer);
    }

    Engine::~Engine()
    {
        delete application;
        delete game;
        delete cameraController;
        delete camera;
        delete input;
        delete renderer;
        delete imGuiRenderer;
        delete window;
        delete graphicsContext;
        delete resourceLoader;
        delete fileSystem;
    }

    void Engine::Run() const
    {
        application->Run();
    }

}
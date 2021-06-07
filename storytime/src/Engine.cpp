#include "Engine.h"
#include "Game.h"

extern storytime::Game* CreateGame(
        storytime::ResourceLoader* resourceLoader,
        storytime::Renderer* renderer,
        storytime::CameraController* cameraController,
        storytime::CoordinateSystem coordinateSystem
);

namespace storytime
{
    Engine::Config::Config()
            : LogLevel(LogLevel::Trace),
              CoordinateSystem(CoordinateSystem::RightDown),
              Window(),
              Graphics()
    {}
}

namespace storytime
{
    Engine::Engine(const Config& config)
    {
        Log::Init(config.LogLevel);
        GraphicsLog::Init(config.Graphics);

        fileSystem = new FileSystem();
        resourceLoader = new ResourceLoader(fileSystem, config.CoordinateSystem);

        graphicsContext = new GraphicsContext(config.Graphics);
        imGuiRenderer = new ImGuiRenderer(graphicsContext);
        window = new Window(config.Window, graphicsContext, imGuiRenderer);
        renderer = new Renderer(resourceLoader, config.CoordinateSystem);

        input = new Input();
        camera = new Camera();
        cameraController = new CameraController(camera, input, config.Window.GetAspectRatio());

        game = CreateGame(resourceLoader, renderer, cameraController, config.CoordinateSystem);
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
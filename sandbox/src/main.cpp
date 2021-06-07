#include "Scene.h"
#include <storytime/Storytime.h>

int main()
{
    storytime::Window::Config windowConfig = {};
    windowConfig.Title = "Sandbox";
    windowConfig.Width = 1280;
    windowConfig.Height = windowConfig.Width / 16 * 12;
    windowConfig.Maximized = false;

    storytime::GraphicsContext::Config graphicsConfig = {};
    graphicsConfig.VersionMajor = 4;
    graphicsConfig.VersionMinor = 1;
    graphicsConfig.GlslVersion = "#version 410";

    storytime::Engine::Config engineConfig = {};
    engineConfig.logLevel = storytime::LogLevel::Trace;
    engineConfig.windowConfig = windowConfig;
    engineConfig.graphicsConfig = graphicsConfig;

    auto engine = new storytime::Engine(engineConfig);
    engine->Run();
    delete engine;
}

class Sandbox : public st::Game
{
private:
    Scene* scene;
public:
    Sandbox(st::CameraController* cameraController, st::Renderer* renderer, st::ResourceLoader* resourceLoader)
            : scene(new Scene(cameraController, renderer, resourceLoader))
    {
    }

    ~Sandbox()
    {
        delete scene;
    }

    void OnUpdate(st::Timestep timestep) override
    {
        scene->OnUpdate(timestep);
    }

    void OnRender() override
    {
        scene->OnRender();
    }

    void OnEvent(const storytime::Event& event) override
    {
        scene->OnEvent(event);
    }
};

st::Game* CreateGame(
        st::CameraController* cameraController,
        st::Input* input,
        st::Window* window,
        st::Renderer* renderer,
        st::ResourceLoader* resourceLoader
)
{
    return new Sandbox(cameraController, renderer, resourceLoader);
}



#include "Scene.h"
#include <storytime/Storytime.h>

int main()
{
    storytime::Engine::Config config = {};
    config.Window.Title = "Sandbox";
    config.Window.Width = 1280;
    config.Window.Height = config.Window.Width / 16 * 12;

    auto engine = new storytime::Engine(config);
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



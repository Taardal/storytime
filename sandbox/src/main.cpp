#include "Scene.h"
#include <storytime/Storytime.h>

int main()
{
    st::Engine::Config config = {};
    config.Window.Title = "Sandbox";
    config.Window.Width = 1280;
    config.Window.Height = config.Window.Width / 16 * 12;

    auto engine = new st::Engine(config);
    engine->Run();
    delete engine;
}

class Sandbox : public st::Game
{
private:
    Scene* scene;
public:
    Sandbox(st::CoordinateSystem coordinateSystem, st::CameraController* cameraController, st::Renderer* renderer, st::ResourceLoader* resourceLoader)
            : scene(new Scene(coordinateSystem, cameraController, renderer, resourceLoader))
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

    void OnEvent(const st::Event& event) override
    {
        scene->OnEvent(event);
    }
};

st::Game* CreateGame(
        st::ResourceLoader* resourceLoader,
        st::Renderer* renderer,
        st::CameraController* cameraController,
        st::CoordinateSystem coordinateSystem
)
{
    return new Sandbox(coordinateSystem, cameraController, renderer, resourceLoader);
}



#include "SandboxLayer.h"
#include <storytime/storytime.h>

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

class Sandbox : public st::Application
{
private:
    SandboxLayer* sandboxLayer;
public:
    Sandbox(
            st::Window* window,
            st::Renderer* renderer,
            st::ImGuiRenderer* imGuiRenderer,
            st::Input* input,
            st::OrthographicCameraController* cameraController,
            storytime::ResourceLoader* resourceLoader
    )
            : Application(window, input, renderer, imGuiRenderer, cameraController),
              sandboxLayer(new SandboxLayer(window, renderer, cameraController, resourceLoader))
    {
        PushLayer(sandboxLayer);
    }

    ~Sandbox()
    {
        delete sandboxLayer;
    }
};

st::Application* CreateApplication(
        st::Window* window,
        st::Renderer* renderer,
        st::ImGuiRenderer* imGuiRenderer,
        st::Input* input,
        st::OrthographicCameraController* cameraController,
        st::ResourceLoader* resourceLoader
)
{
    return new Sandbox(window, renderer, imGuiRenderer, input, cameraController, resourceLoader);
}



#include "EditorLayer.h"
#include <storytime/Storytime.h>

int main()
{
    st::Window::Config windowConfig = {};
    windowConfig.Title = "Editor";
    windowConfig.Width = 1280;
    windowConfig.Height = windowConfig.Width / 16 * 12;
    windowConfig.Maximized = false;

    st::GraphicsContext::Config graphicsConfig = {};
    graphicsConfig.VersionMajor = 4;
    graphicsConfig.VersionMinor = 1;
    graphicsConfig.GlslVersion = "#version 410";

    st::Engine::Config engineConfig = {};
    engineConfig.logLevel = st::LogLevel::Trace;
    engineConfig.windowConfig = windowConfig;
    engineConfig.graphicsConfig = graphicsConfig;

#if 0
    auto engine = new st::Engine(engineConfig);
    engine->Run();
    delete engine;
#endif
}

#if 0
namespace storytime
{
    class Editor : public Application
    {
    private:
        EditorLayer* editorLayer;
    public:
        Editor(
                Window* window,
                Renderer* renderer,
                ImGuiRenderer* imGuiRenderer,
                Input* input,
                CameraController* cameraController,
                ResourceLoader* resourceLoader
        )
                : Application(window, input, renderer, imGuiRenderer, cameraController),
                  editorLayer(new EditorLayer(window, renderer, cameraController, resourceLoader))
        {
            PushLayer(editorLayer);
        }

        ~Editor()
        {
            delete editorLayer;
        }
    };
}

st::Application* CreateApplication(
        st::Window* window,
        st::Renderer* renderer,
        st::ImGuiRenderer* imGuiRenderer,
        st::Input* input,
        st::CameraController* cameraController,
        st::ResourceLoader* resourceLoader
)
{
    return new st::Editor(window, renderer, imGuiRenderer, input, cameraController, resourceLoader);
}
#endif

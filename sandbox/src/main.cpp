#include "storytime/Storytime.h"

int main()
{
    storytime::Window::Config windowConfig = {};
    windowConfig.Title = "Sandbox";
    windowConfig.Width = 1280;
    windowConfig.Height = windowConfig.Width / 16 * 12;
    windowConfig.Maximized = true;

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

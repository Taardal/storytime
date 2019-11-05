#include "darkle/Darkle.h"
#include "SandboxLayer.h"

Darkle::Window::Config getWindowConfig() {
    Darkle::Window::Config config = {};
    config.title = "Sandbox";
    config.width = 1024;
    config.height = config.width / 16 * 12;
    return config;
}

Darkle::GraphicsContext::Config getGraphicsConfig() {
    Darkle::GraphicsContext::Config config = {};
    config.openGLVersionMajor = 4;
    config.openGLVersionMinor = 1;
    return config;
}

Darkle::Engine::Config getConfig() {
    Darkle::Engine::Config config = {};
    config.logLevel = Darkle::LogLevel::Trace;
    config.windowConfig = getWindowConfig();
    config.graphicsConfig = getGraphicsConfig();
    return config;
}

int main() {
    auto engine = new Darkle::Engine(getConfig());
    engine->pushLayer(new Sandbox::SandboxLayer("SandboxLayer"));
    engine->run();
    delete engine;
    return 0;
}

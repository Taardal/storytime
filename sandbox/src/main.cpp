#include "storytime/Storytime.h"
#include <iostream>

storytime::Window::Config getWindowConfig() {
    storytime::Window::Config config = {};
    config.title = "Sandbox";
    config.width = 1280;
    config.height = config.width / 16 * 12;
    return config;
}

storytime::GraphicsContext::Config getGraphicsConfig() {
    storytime::GraphicsContext::Config config = {};
    config.versionMajor = 4;
    config.versionMinor = 1;
    return config;
}

storytime::Engine::Config getConfig() {
    storytime::Engine::Config config = {};
    config.logLevel = storytime::LogLevel::Trace;
    config.windowConfig = getWindowConfig();
    config.graphicsConfig = getGraphicsConfig();
    return config;
}

int main() {
    auto engine = new storytime::Engine(getConfig());
    engine->run();
    delete engine;
}

#include "storytime/Storytime.h"
#include "storytiled/Storytiled.h"
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
    config.openGLVersionMajor = 4;
    config.openGLVersionMinor = 1;
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
    /*
    auto engine = new storytime::Engine(getConfig());
    engine->run();
    delete engine;
    */

    const std::string& worldJson = st::FileReader("assets/worlds/world_pixelcave.json").read();
    std::cout << worldJson << std::endl;
    sti::World world = sti::World::fromJson(worldJson);
    std::cout << world.tiledVersion << std::endl;
}

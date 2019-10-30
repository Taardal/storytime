#include "darkle/Darkle.h"

Darkle::Engine::Config getConfig();

int main() {
    auto engine = new Darkle::Engine(getConfig());
    engine->run();
    delete engine;
    return 0;
}

Darkle::Engine::Config getConfig() {
    const Darkle::Window::Config& windowConfig = {
        "Sandbox",
                1024,
                768
    };
    const Darkle::Engine::Config& config = {
            Darkle::LogLevel::Trace,
            windowConfig
    };
    return config;
}
#include "darkle/Darkle.h"

int main() {
    const Darkle::Config& config = {
            Darkle::LogLevel::Trace,
            "Sandbox"
    };
    auto engine = new Darkle::Engine(config);
    engine->run();
    delete engine;
    return 0;
}
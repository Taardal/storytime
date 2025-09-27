#pragma once

namespace Storytime {
    class App {
    public:
        virtual ~App() = default;

        virtual void update(f64 timestep) = 0;

        virtual void render() = 0;

        virtual void render_imgui() = 0;
    };
}
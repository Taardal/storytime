#pragma once

#include "graphics/camera.h"
#include "graphics/renderer.h"

namespace Storytime {
    class App {
    public:
        virtual ~App() = default;

        virtual void on_update(Camera* camera, f64 timestep) = 0;

        virtual void on_render(Renderer* renderer) = 0;

        virtual void on_imgui_render() {}
    };
}

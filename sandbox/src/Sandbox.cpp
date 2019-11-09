#include "Sandbox.h"
#include "TriangleLayer.h"

namespace sandbox {

    Sandbox::Sandbox(storytime::Window* window, storytime::Renderer* renderer) : Application(window, renderer) {
        pushLayer(new TriangleLayer());
    }
}

storytime::Application* CreateApplication(storytime::Window* window, storytime::Renderer* renderer) {
    return new sandbox::Sandbox(window, renderer);
}

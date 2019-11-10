#include "Sandbox.h"
#include "TriangleLayer.h"
#include "storytime/Storytime.h"

namespace sandbox {

    Sandbox::Sandbox(storytime::Window* window, storytime::Renderer* renderer, storytime::Input* input) : Application(window, renderer, input) {
        float aspectRatio = window->getConfig().getAspectRatio();
        storytime::OrthographicCamera camera;
        storytime::OrthographicCameraController cameraController(camera, aspectRatio);
        pushLayer(new TriangleLayer(cameraController));
    }
}

storytime::Application* CreateApplication(storytime::Window* window, storytime::Renderer* renderer, storytime::Input* input) {
    return new sandbox::Sandbox(window, renderer, input);
}

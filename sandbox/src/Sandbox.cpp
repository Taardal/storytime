#include "Sandbox.h"
#include "TriangleLayer.h"
#include "storytime/Storytime.h"

namespace sandbox
{
    Sandbox::Sandbox(st::Window* window, st::Renderer* renderer, st::Input* input, st::OrthographicCameraController* cameraController)
            : Application(window, renderer, input, cameraController)
    {
        pushLayer(new TriangleLayer(cameraController));
    }
}

st::Application* CreateApplication(st::Window* window, st::Renderer* renderer, st::Input* input, st::OrthographicCameraController* cameraController)
{
    return new sandbox::Sandbox(window, renderer, input, cameraController);
}

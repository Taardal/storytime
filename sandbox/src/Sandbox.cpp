#include "Sandbox.h"
#include "SandboxLayer.h"
#include "storytime/Storytime.h"

Sandbox::Sandbox(st::Window* window, st::Renderer* renderer, st::Input* input, st::OrthographicCameraController* cameraController, storytime::ResourceLoader* resourceLoader)
        : Application(window, renderer, input, cameraController)
{
    pushLayer(new SandboxLayer(cameraController, resourceLoader));
}

st::Application* CreateApplication(st::Window* window, st::Renderer* renderer, st::Input* input, st::OrthographicCameraController* cameraController, st::ResourceLoader* resourceLoader)
{
    return new Sandbox(window, renderer, input, cameraController, resourceLoader);
}

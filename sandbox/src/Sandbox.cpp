#include "Sandbox.h"
#include "SandboxLayer.h"
#include "storytime/Storytime.h"

Sandbox::Sandbox(st::Window* window, st::Renderer* renderer, st::ImGuiRenderer* imGuiRenderer, st::Input* input, st::OrthographicCameraController* cameraController, storytime::ResourceLoader* resourceLoader)
        : Application(window, renderer, imGuiRenderer, input, cameraController)
{
    PushLayer(new SandboxLayer(cameraController, resourceLoader));
}

st::Application* CreateApplication(st::Window* window, st::Renderer* renderer, st::ImGuiRenderer* imGuiRenderer, st::Input* input, st::OrthographicCameraController* cameraController, st::ResourceLoader* resourceLoader)
{
    return new Sandbox(window, renderer, imGuiRenderer, input, cameraController, resourceLoader);
}

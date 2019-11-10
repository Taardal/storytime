#include <graphics/OrthographicCamera.h>
#include "Sandbox.h"
#include "TriangleLayer.h"
#include "storytime/Storytime.h"

namespace sandbox {

    Sandbox::Sandbox(storytime::Window* window, storytime::Renderer* renderer) : Application(window, renderer) {
        float aspectRatio = window->getConfig().getAspectRatio();
        float zoomLevel = 1.0f;
        float top = zoomLevel;
        float bottom = -zoomLevel;
        float left = -aspectRatio * zoomLevel;
        float right = aspectRatio * zoomLevel;
        const storytime::OrthographicCamera camera(top, bottom, left, right);
        const storytime::OrthographicCameraController& cameraController(camera, aspectRatio);
        pushLayer(new TriangleLayer(cameraController));
    }
}

storytime::Application* CreateApplication(storytime::Window* window, storytime::Renderer* renderer) {
    return new sandbox::Sandbox(window, renderer);
}

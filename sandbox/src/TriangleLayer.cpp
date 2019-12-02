#include "system/Log.h"
#include "TriangleLayer.h"

namespace sandbox {

    TriangleLayer::TriangleLayer(st::OrthographicCameraController* cameraController)
            : Layer("TriangleLayer"), cameraController(cameraController) {
        ST_TRACE(ST_TAG, "Created");
    }

    TriangleLayer::~TriangleLayer() {
        ST_TRACE(ST_TAG, "Destroyed");
    }

    void TriangleLayer::onAttach() {
    }

    void TriangleLayer::onDetach() {
    }

    void TriangleLayer::onUpdate(st::Timestep timestep, st::Renderer* renderer, st::Input* input) {
        cameraController->onUpdate(timestep, input);
        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        glm::vec2 size = { 0.8f, 0.8f };
        glm::vec4 color = { 0.8f, 0.2f, 0.3f, 1.0f };
        renderer->drawQuad(position, size, color);
    }

    void TriangleLayer::onEvent(const st::Event& event) {
        cameraController->onEvent(event);
    }

}

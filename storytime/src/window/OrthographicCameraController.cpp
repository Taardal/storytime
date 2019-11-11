#include "OrthographicCameraController.h"
#include "window/events/WindowEvent.h"
#include "window/events/MouseEvent.h"
#include "system/Log.h"
#include <cmath>

namespace storytime {

    OrthographicCameraController::OrthographicCameraController(OrthographicCamera* camera, float aspectRatio)
            : camera(camera), aspectRatio(aspectRatio), zoomLevel(1.0f), rotation(true),
            cameraPosition(0.0f, 0.0f, 0.0f), cameraRotation(0.0f), cameraTranslationSpeed(5.0f), cameraRotationSpeed(180.0f) {
        ST_TRACE(ST_TAG, "Creating");
        setCameraProjection();
        ST_TRACE(ST_TAG, "Created");
    }

    OrthographicCameraController::~OrthographicCameraController() {
        ST_TRACE(ST_TAG, "Destroyed");
    }

    OrthographicCamera* OrthographicCameraController::getCamera() const {
        return camera;
    }

    void OrthographicCameraController::onUpdate(Timestep timestep, Input* input) {
        if (input->isKeyPressed(KeyCode::KEY_A)) {
            cameraPosition.x -= cos(glm::radians(cameraRotation)) * cameraTranslationSpeed * timestep;
            cameraPosition.y -= sin(glm::radians(cameraRotation)) * cameraTranslationSpeed * timestep;
        } else if (input->isKeyPressed(KeyCode::KEY_D)) {
            cameraPosition.x += cos(glm::radians(cameraRotation)) * cameraTranslationSpeed * timestep;
            cameraPosition.y += sin(glm::radians(cameraRotation)) * cameraTranslationSpeed * timestep;
        }
        if (input->isKeyPressed(KeyCode::KEY_W)) {
            cameraPosition.x += -sin(glm::radians(cameraRotation)) * cameraTranslationSpeed * timestep;
            cameraPosition.y += cos(glm::radians(cameraRotation)) * cameraTranslationSpeed * timestep;
        } else if (input->isKeyPressed(KeyCode::KEY_S)) {
            cameraPosition.x -= -sin(glm::radians(cameraRotation)) * cameraTranslationSpeed * timestep;
            cameraPosition.y -= cos(glm::radians(cameraRotation)) * cameraTranslationSpeed * timestep;
        }
        if (rotation) {
            if (input->isKeyPressed(KeyCode::KEY_Q)) {
                cameraRotation += cameraRotationSpeed * timestep;
            }
            if (input->isKeyPressed(KeyCode::KEY_E)) {
                cameraRotation -= cameraRotationSpeed * timestep;
            }
            camera->setRotation(cameraRotation);
        }
        camera->setPosition(cameraPosition);
        cameraTranslationSpeed = zoomLevel;
    }

    void OrthographicCameraController::onEvent(const Event& event) {
        if (event.getType() == EventType::MouseScrolled) {
            const auto* mouseScrolledEvent = (MouseScrolledEvent*) &event;
            zoomLevel -= mouseScrolledEvent->getYOffset() * 0.25f;
            zoomLevel = std::fmax(zoomLevel, 0.25f);
            setCameraProjection();
        }
        if (event.getType() == EventType::WindowResize) {
            const auto* windowResizeEvent = (WindowResizeEvent*) &event;
            aspectRatio = (float) windowResizeEvent->getWidth() / (float) windowResizeEvent->getHeight();
            setCameraProjection();
        }
    }

    void OrthographicCameraController::setCameraProjection() {
        float top = zoomLevel;
        float bottom = -zoomLevel;
        float left = -aspectRatio * zoomLevel;
        float right = aspectRatio * zoomLevel;
        camera->setProjection(top, bottom, left, right);
    }

}


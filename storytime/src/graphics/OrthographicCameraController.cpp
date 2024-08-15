#include "OrthographicCameraController.h"
#include "window/window_event.h"
#include "window/mouse_event.h"
#include "system/log.h"
#include <cmath>

namespace Storytime
{
    OrthographicCameraController::OrthographicCameraController(OrthographicCamera* camera, float aspectRatio)
            : camera(camera),
              aspectRatio(aspectRatio),
              cameraPosition(0.0f, 0.0f, 0.0f),
              cameraRotation(0.0f),
              cameraTranslationSpeed(5.0f),
              cameraRotationSpeed(180.0f),
              zoomLevel(1.0f),
              rotationEnabled(true)
    {
        SetCameraProjection();
    }

    OrthographicCamera* OrthographicCameraController::GetCamera() const
    {
        return camera;
    }

    void OrthographicCameraController::SetZoomLevel(float zoomLevel)
    {
        this->zoomLevel = zoomLevel;
        SetCameraProjection();
    }

    void OrthographicCameraController::OnUpdate(f32 timestep, Input* input)
    {
        if (input->IsKeyPressed(KeyCode::KEY_A))
        {
            cameraPosition.x -= cos(glm::radians(cameraRotation)) * cameraTranslationSpeed * timestep;
            cameraPosition.y -= sin(glm::radians(cameraRotation)) * cameraTranslationSpeed * timestep;
        }
        else if (input->IsKeyPressed(KeyCode::KEY_D))
        {
            cameraPosition.x += cos(glm::radians(cameraRotation)) * cameraTranslationSpeed * timestep;
            cameraPosition.y += sin(glm::radians(cameraRotation)) * cameraTranslationSpeed * timestep;
        }
        if (input->IsKeyPressed(KeyCode::KEY_W))
        {
            cameraPosition.x += -sin(glm::radians(cameraRotation)) * cameraTranslationSpeed * timestep;
            cameraPosition.y += cos(glm::radians(cameraRotation)) * cameraTranslationSpeed * timestep;
        }
        else if (input->IsKeyPressed(KeyCode::KEY_S))
        {
            cameraPosition.x -= -sin(glm::radians(cameraRotation)) * cameraTranslationSpeed * timestep;
            cameraPosition.y -= cos(glm::radians(cameraRotation)) * cameraTranslationSpeed * timestep;
        }
        if (rotationEnabled)
        {
            if (input->IsKeyPressed(KeyCode::KEY_Q))
            {
                cameraRotation += cameraRotationSpeed * timestep;
            }
            if (input->IsKeyPressed(KeyCode::KEY_E))
            {
                cameraRotation -= cameraRotationSpeed * timestep;
            }
            camera->SetRotation(cameraRotation);
        }
        camera->SetPosition(cameraPosition);
        cameraTranslationSpeed = zoomLevel;
    }

    void OrthographicCameraController::OnEvent(const Event& event)
    {
        if (event.get_type() == EventType::MouseScroll)
        {
            const auto* mouseScrolledEvent = (MouseScrollEvent*) &event;
            zoomLevel -= mouseScrolledEvent->y_offset * 0.25f;
            zoomLevel = std::fmax(zoomLevel, 0.25f);
            SetCameraProjection();
        }
        if (event.get_type() == EventType::WindowResize)
        {
            const auto* windowResizeEvent = (WindowResizeEvent*) &event;
            aspectRatio = (float) windowResizeEvent->width / (float) windowResizeEvent->height;
            SetCameraProjection();
        }
    }

    void OrthographicCameraController::Resize(uint32_t width, uint32_t height)
    {
        aspectRatio = width / height;
        SetCameraProjection();
    }

    void OrthographicCameraController::SetCameraProjection()
    {
        float top = zoomLevel;
        float bottom = -zoomLevel;
        float left = -aspectRatio * zoomLevel;
        float right = aspectRatio * zoomLevel;
        camera->SetProjection(top, bottom, left, right);
    }

}


#include "OrthographicCameraController.h"
#include "window/events/WindowEvent.h"
#include "window/events/MouseEvent.h"
#include "system/Log.h"
#include <cmath>

namespace storytime
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
        ST_LOG_TRACE(ST_TAG, "Created");
    }

    OrthographicCameraController::~OrthographicCameraController()
    {
        ST_LOG_TRACE(ST_TAG, "Destroyed");
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

    void OrthographicCameraController::OnUpdate(Timestep timestep, Input* input)
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
        if (event.GetType() == EventType::MouseScrolled)
        {
            const auto* mouseScrolledEvent = (MouseScrolledEvent*) &event;
            zoomLevel -= mouseScrolledEvent->GetYOffset() * 0.25f;
            zoomLevel = std::fmax(zoomLevel, 0.25f);
            SetCameraProjection();
        }
        if (event.GetType() == EventType::WindowResize)
        {
            const auto* windowResizeEvent = (WindowResizeEvent*) &event;
            aspectRatio = (float) windowResizeEvent->GetWidth() / (float) windowResizeEvent->GetHeight();
            SetCameraProjection();
        }
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


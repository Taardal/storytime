#include "CameraController.h"
#include "window/events/WindowEvent.h"
#include "window/events/MouseEvent.h"
#include "system/Log.h"
#include <cmath>

namespace storytime
{
    CameraController::CameraController(Camera* camera, Input* input, float aspectRatio)
            : camera(camera),
              input(input),
              aspectRatio(aspectRatio),
              cameraPosition(0.0f, 0.0f, 0.0f),
              cameraRotation(0.0f),
              cameraTranslationSpeed(5.0f),
              cameraRotationSpeed(180.0f),
              zoomLevel(1.0f),
              rotationEnabled(true)
    {
        UpdateCameraProjection();
    }

    Camera* CameraController::GetCamera() const
    {
        return camera;
    }

    void CameraController::SetPosition(float x, float y)
    {
        cameraPosition.x = x;
        cameraPosition.y = y;
        camera->SetPosition(cameraPosition);
    }

    void CameraController::SetZoomLevel(float zoomLevel)
    {
        this->zoomLevel = zoomLevel;
        UpdateCameraProjection();
    }

    void CameraController::OnUpdate(Timestep timestep)
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

    void CameraController::OnEvent(const Event& event)
    {
        if (event.GetType() == EventType::MouseScrolled)
        {
            const auto* mouseScrolledEvent = (MouseScrolledEvent*) &event;
            zoomLevel -= mouseScrolledEvent->GetYOffset() * 0.25f;
            zoomLevel = std::fmax(zoomLevel, 0.25f);
            UpdateCameraProjection();
        }
        if (event.GetType() == EventType::WindowResize)
        {
            const auto* windowResizeEvent = (WindowResizeEvent*) &event;
            aspectRatio = (float) windowResizeEvent->GetWidth() / (float) windowResizeEvent->GetHeight();
            UpdateCameraProjection();
        }
    }

    void CameraController::Resize(uint32_t width, uint32_t height)
    {
        aspectRatio = width / height;
        UpdateCameraProjection();
    }

    void CameraController::UpdateCameraProjection()
    {
        float top = zoomLevel;
        float bottom = -zoomLevel;
        float left = -aspectRatio * zoomLevel;
        float right = aspectRatio * zoomLevel;
        camera->SetProjection(top, bottom, left, right);
    }

    CameraController::Size CameraController::GetSize() const
    {
        int32_t width = aspectRatio * zoomLevel * 2;
        int32_t height = zoomLevel * 2;
        return { width, height };
    }

}


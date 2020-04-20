#pragma once

#include "system/Core.h"
#include "system/Timestep.h"
#include "window/Input.h"
#include "window/events/Event.h"
#include "graphics/OrthographicCamera.h"

namespace storytime
{
    class OrthographicCameraController
    {
    private:
        OrthographicCamera* camera;
        float aspectRatio;
        glm::vec3 cameraPosition = { 0.0f, 0.0f, 0.0f };
        float cameraRotation = 0.0f;
        float cameraTranslationSpeed = 5.0f;
        float cameraRotationSpeed = 180.0f;
        float zoomLevel = 1.0f;
        bool rotation = true;

    public:
        OrthographicCameraController(OrthographicCamera* camera, float aspectRatio);

        ~OrthographicCameraController();

        [[nodiscard]] OrthographicCamera* GetCamera() const;

        void OnUpdate(Timestep timestep, Input* input);

        void OnEvent(const Event& event);

    private:
        void SetCameraProjection();
    };

}



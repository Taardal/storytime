#pragma once

#include "system/Core.h"
#include "system/Timestep.h"
#include "window/Input.h"
#include "window/events/Event.h"
#include "graphics/OrthographicCamera.h"

namespace Storytime
{
    class OrthographicCameraController
    {
    private:
        OrthographicCamera* camera;
        float aspectRatio;
        glm::vec3 cameraPosition;
        float cameraRotation;
        float cameraTranslationSpeed;
        float cameraRotationSpeed;
        float zoomLevel;
        bool rotationEnabled;

    public:
        OrthographicCameraController(OrthographicCamera* camera, float aspectRatio);

        ~OrthographicCameraController();

        [[nodiscard]] OrthographicCamera* GetCamera() const;

        void SetZoomLevel(float zoomLevel);

        void OnUpdate(Timestep timestep, Input* input);

        void OnEvent(const Event& event);

        void Resize(uint32_t width, uint32_t height);

    private:
        void SetCameraProjection();
    };

}



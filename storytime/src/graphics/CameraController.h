#pragma once

#include "system/Core.h"
#include "system/Timestep.h"
#include "window/Input.h"
#include "window/events/Event.h"
#include "graphics/Camera.h"

namespace storytime
{
    class CameraController
    {
    public:
        struct Size
        {
            int32_t Width;
            int32_t Height;
        };

    private:
        Camera* camera;
        Input* input;
        float aspectRatio;
        glm::vec3 cameraPosition;
        float cameraRotation;
        float cameraTranslationSpeed;
        float cameraRotationSpeed;
        float zoomLevel;
        bool rotationEnabled;

    public:
        CameraController(Camera* camera, Input* input, float aspectRatio);

        [[nodiscard]] Camera* GetCamera() const;

        Size GetSize() const;

        void SetPosition(float x, float y);

        void SetZoomLevel(float zoomLevel);

        void OnUpdate(Timestep timestep);

        void OnEvent(const Event& event);

        void Resize(uint32_t width, uint32_t height);

    private:
        void UpdateCameraProjection();
    };

}



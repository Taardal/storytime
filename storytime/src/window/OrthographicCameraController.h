#pragma once

#include "system/Core.h"
#include "system/Timestep.h"
#include "window/Input.h"
#include "window/events/Event.h"
#include "graphics/OrthographicCamera.h"

namespace storytime {

    class OrthographicCameraController {
    private:
        OrthographicCamera* camera;
        float aspectRatio;
        float zoomLevel;
        bool rotation;
        glm::vec3 cameraPosition;
        float cameraRotation;
        float cameraTranslationSpeed;
        float cameraRotationSpeed;

    public:
        OrthographicCameraController(OrthographicCamera* camera, float aspectRatio);

        ~OrthographicCameraController();

        [[nodiscard]] OrthographicCamera* getCamera() const;

        void onUpdate(Timestep timestep, Input* input);

        void onEvent(const Event& event);

    private:
        void setCameraProjection();
    };

}



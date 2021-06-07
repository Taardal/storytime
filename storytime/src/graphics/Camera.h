#pragma once

#include <glm/glm.hpp>

namespace storytime
{
    class Camera
    {
    private:
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::mat4 viewProjectionMatrix;
        glm::vec3 position;
        float rotation;

    public:
        Camera();

        const glm::mat4& GetView() const;

        const glm::mat4& GetProjection() const;

        const glm::mat4& GetViewProjection() const;

        const glm::vec3& GetPosition() const;

        void SetPosition(const glm::vec3& position);

        float GetRotation() const;

        void SetRotation(float rotation);

        void SetProjection(float top, float bottom, float left, float right);

    private:
        void UpdateViewMatrix();

        glm::mat4 GetTranslationMatrix() const;

        glm::mat4 GetRotationMatrix() const;
    };

}



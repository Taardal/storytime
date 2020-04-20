#pragma once

#include <glm/glm.hpp>

namespace storytime
{
    class OrthographicCamera
    {
    private:
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::mat4 viewProjectionMatrix;
        glm::vec3 position;
        float rotation;

    public:
        OrthographicCamera();

        virtual ~OrthographicCamera();

        [[nodiscard]] const glm::mat4& GetView() const;

        [[nodiscard]] const glm::mat4& GetProjection() const;

        [[nodiscard]] const glm::mat4& GetViewProjection() const;

        [[nodiscard]] const glm::vec3& GetPosition() const;

        void SetPosition(const glm::vec3& position);

        [[nodiscard]] float GetRotation() const;

        void SetRotation(float rotation);

        void SetProjection(float top, float bottom, float left, float right);

    private:
        void UpdateViewMatrix();

        [[nodiscard]] glm::mat4 GetTranslationMatrix() const;

        [[nodiscard]] glm::mat4 GetRotationMatrix() const;
    };

}



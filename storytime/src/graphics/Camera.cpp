#include "Camera.h"
#include "system/Log.h"
#include <glm/gtc/matrix_transform.hpp>

namespace storytime
{
    constexpr float Z_NEAR = -1.0f;
    constexpr float Z_FAR = 1.0f;

    Camera::Camera()
            : viewMatrix(1.0f), projectionMatrix(1.0f), viewProjectionMatrix(1.0f), position(0.0f, 0.0f, 0.0f), rotation(0.0f)
    {
    }

    const glm::mat4& Camera::GetView() const
    {
        return viewMatrix;
    }

    const glm::mat4& Camera::GetProjection() const
    {
        return projectionMatrix;
    }

    const glm::mat4& Camera::GetViewProjection() const
    {
        return viewProjectionMatrix;
    }

    const glm::vec3& Camera::GetPosition() const
    {
        return position;
    }

    void Camera::SetPosition(const glm::vec3& position)
    {
        this->position = position;
        UpdateViewMatrix();
    }

    float Camera::GetRotation() const
    {
        return rotation;
    }

    void Camera::SetRotation(float rotation)
    {
        this->rotation = rotation;
        UpdateViewMatrix();
    }

    void Camera::SetProjection(float top, float bottom, float left, float right)
    {
        projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    void Camera::UpdateViewMatrix()
    {
        viewMatrix = GetTranslationMatrix() * GetRotationMatrix();
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    glm::mat4 Camera::GetTranslationMatrix() const
    {
        return glm::translate(glm::mat4(1.0f), position);
    }

    glm::mat4 Camera::GetRotationMatrix() const
    {
        float angle = glm::radians(rotation);
        const glm::vec3& zAxis = glm::vec3(0, 0, 1);
        return glm::rotate(glm::mat4(1.0f), angle, zAxis);
    }

}

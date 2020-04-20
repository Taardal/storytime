#include "OrthographicCamera.h"
#include "system/Log.h"
#include <glm/gtc/matrix_transform.hpp>

namespace storytime
{
    constexpr float Z_NEAR = -1.0f;
    constexpr float Z_FAR = 1.0f;

    OrthographicCamera::OrthographicCamera()
            : viewMatrix(1.0f), projectionMatrix(1.0f), viewProjectionMatrix(1.0f), position(0.0f, 0.0f, 0.0f), rotation(0.0f)
    {
        ST_LOG_TRACE(ST_TAG, "Created");
    }

    OrthographicCamera::~OrthographicCamera()
    {
        ST_LOG_TRACE(ST_TAG, "Destroyed");
    }

    const glm::mat4& OrthographicCamera::GetView() const
    {
        return viewMatrix;
    }

    const glm::mat4& OrthographicCamera::GetProjection() const
    {
        return projectionMatrix;
    }

    const glm::mat4& OrthographicCamera::GetViewProjection() const
    {
        return viewProjectionMatrix;
    }

    const glm::vec3& OrthographicCamera::GetPosition() const
    {
        return position;
    }

    void OrthographicCamera::SetPosition(const glm::vec3& position)
    {
        this->position = position;
        UpdateViewMatrix();
    }

    float OrthographicCamera::GetRotation() const
    {
        return rotation;
    }

    void OrthographicCamera::SetRotation(float rotation)
    {
        this->rotation = rotation;
        UpdateViewMatrix();
    }

    void OrthographicCamera::SetProjection(float top, float bottom, float left, float right)
    {
        projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    void OrthographicCamera::UpdateViewMatrix()
    {
        viewMatrix = GetTranslationMatrix() * GetRotationMatrix();
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    glm::mat4 OrthographicCamera::GetTranslationMatrix() const
    {
        return glm::translate(glm::mat4(1.0f), position);
    }

    glm::mat4 OrthographicCamera::GetRotationMatrix() const
    {
        float angle = glm::radians(rotation);
        glm::vec3 zAxis = glm::vec3(0, 0, 1);
        return glm::rotate(glm::mat4(1.0f), angle, zAxis);
    }

}

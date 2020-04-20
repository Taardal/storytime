#include "OrthographicCamera.h"
#include "system/Log.h"
#include <glm/gtc/matrix_transform.hpp>

namespace storytime {

    constexpr float Z_NEAR = -1.0f;
    constexpr float Z_FAR = 1.0f;

    OrthographicCamera::OrthographicCamera()
            : viewMatrix(1.0f), projectionMatrix(1.0f), viewProjectionMatrix(1.0f), position(0.0f, 0.0f, 0.0f), rotation(0.0f) {
        ST_LOG_TRACE(ST_TAG, "Destroying");
    }

    OrthographicCamera::~OrthographicCamera() {
        ST_LOG_TRACE(ST_TAG, "Destroyed");
    }

    const glm::mat4& OrthographicCamera::getView() const {
        return viewMatrix;
    }

    const glm::mat4& OrthographicCamera::getProjection() const {
        return projectionMatrix;
    }

    const glm::mat4& OrthographicCamera::getViewProjection() const {
        return viewProjectionMatrix;
    }

    const glm::vec3& OrthographicCamera::getPosition() const {
        return position;
    }

    void OrthographicCamera::setPosition(const glm::vec3& position) {
        this->position = position;
        updateViewMatrix();
    }

    float OrthographicCamera::getRotation() const {
        return rotation;
    }

    void OrthographicCamera::setRotation(float rotation) {
        this->rotation = rotation;
        updateViewMatrix();
    }

    void OrthographicCamera::setProjection(float top, float bottom, float left, float right) {
        projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    void OrthographicCamera::updateViewMatrix() {
        viewMatrix = getTranslationMatrix() * getRotationMatrix();
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    glm::mat4 OrthographicCamera::getTranslationMatrix() const {
        return glm::translate(glm::mat4(1.0f), position);
    }

    glm::mat4 OrthographicCamera::getRotationMatrix() const {
        float angle = glm::radians(rotation);
        glm::vec3 zAxis = glm::vec3(0, 0, 1);
        return glm::rotate(glm::mat4(1.0f), angle, zAxis);
    }

}

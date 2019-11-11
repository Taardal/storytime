#pragma once

#include <glm/glm.hpp>

namespace storytime {

    class OrthographicCamera {
    private:
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::mat4 viewProjectionMatrix;
        glm::vec3 position;
        float rotation;

    public:
        OrthographicCamera();

        virtual ~OrthographicCamera();

        [[nodiscard]] const glm::mat4& getView() const;

        [[nodiscard]] const glm::mat4& getProjection() const;

        [[nodiscard]] const glm::mat4& getViewProjection() const;

        [[nodiscard]] const glm::vec3& getPosition() const;

        void setPosition(const glm::vec3& position);

        [[nodiscard]] float getRotation() const;

        void setRotation(float rotation);

        void setProjection(float top, float bottom, float left, float right);

    private:
        void updateViewMatrix();

        [[nodiscard]] glm::mat4 getTranslationMatrix() const;

        [[nodiscard]] glm::mat4 getRotationMatrix() const;
    };

}



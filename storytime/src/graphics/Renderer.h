#pragma once

#include "OrthographicCamera.h"
#include "VertexArray.h"
#include "Shader.h"
#include "system/Core.h"
#include <glm/glm.hpp>

namespace storytime {

    class Renderer {
    private:
        Ref<VertexArray> vertexArray;
        Ref<Shader> shader;

    public:
        Renderer();

        ~Renderer();

        void beginScene(OrthographicCamera* camera) const;

        void drawElements() const;

        void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    };

}

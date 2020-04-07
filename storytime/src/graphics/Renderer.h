#pragma once

#include "OrthographicCamera.h"
#include "VertexArray.h"
#include "Shader.h"
#include "system/Core.h"
#include <glm/glm.hpp>
#include <system/ResourceLoader.h>

namespace storytime {

    class Renderer {
    private:
        Ref<VertexArray> vertexArray;
        Ref<Shader> shader;

    public:
        explicit Renderer(ResourceLoader* pLoader);

        ~Renderer();

        void setViewport(uint32_t width, uint32_t height, uint32_t x = 0, uint32_t y = 0) const;

        void beginScene(OrthographicCamera* camera) const;

        void drawElements() const;

        void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    };

}

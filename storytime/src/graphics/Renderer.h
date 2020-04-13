#pragma once

#include "OrthographicCamera.h"
#include "VertexArray.h"
#include "Shader.h"
#include "system/Core.h"
#include <glm/glm.hpp>
#include <system/ResourceLoader.h>

namespace storytime
{
    struct Quad
    {
        glm::vec3 Position;
        glm::vec2 Size;
        glm::vec4 Color;
        float RotationInDegrees;
    };

    class Renderer
    {
    private:
        struct Vertex
        {
            glm::vec3 Position;
            glm::vec4 Color;
        };

        static const uint32_t VERTICES_PER_QUAD;
        static const uint32_t INDICES_PER_QUAD;
        static const uint32_t QUADS_PER_BATCH;
        static const uint32_t VERTICES_PER_BATCH;
        static const uint32_t INDICES_PER_BATCH;

        Ref<VertexArray> vertexArray;
        Ref<VertexBuffer> vertexBuffer;
        Ref<Shader> shader;
        Vertex* vertices;
        uint32_t* indices;
        uint32_t vertexCount;
        uint32_t indexCount;

    public:
        explicit Renderer(ResourceLoader* pLoader);

        ~Renderer();

        void SetViewport(uint32_t width, uint32_t height, uint32_t x = 0, uint32_t y = 0) const;

        void BeginScene(OrthographicCamera* camera);

        void DrawQuad(const Quad& quad);

        void EndScene() const;

    private:
        void DrawIndexed() const;
    };
}

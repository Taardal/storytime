#pragma once

#include "OrthographicCamera.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "system/Core.h"
#include <glm/glm.hpp>
#include <system/ResourceLoader.h>
#include <vector>

namespace storytime
{
    struct Quad
    {
        Ref<Texture> Texture;
        glm::vec3 Position;
        glm::vec2 Size;
        glm::vec4 Color;
        float RotationInDegrees;
        float TilingFactor;

        Quad();
    };

    class Renderer
    {
    public:
        struct Statistics
        {
            uint32_t DrawCalls;
            uint32_t QuadCount;

            Statistics();

            [[nodiscard]] uint32_t GetVertexCount() const;

            [[nodiscard]] uint32_t GetIndexCount() const;
        };

    private:
        struct Vertex
        {
            glm::vec3 Position;
            glm::vec4 Color;
            glm::vec2 TextureCoordinate;
            float TextureIndex;
            float TilingFactor;

            Vertex();
        };

        static const uint32_t VERTICES_PER_QUAD;
        static const uint32_t INDICES_PER_QUAD;
        static const uint32_t QUADS_PER_BATCH;
        static const uint32_t VERTICES_PER_BATCH;
        static const uint32_t INDICES_PER_BATCH;
        static const uint32_t MAX_TEXTURE_SLOTS;

        Ref<VertexArray> vertexArray;
        Ref<VertexBuffer> vertexBuffer;
        Ref<Shader> shader;
        Ref<Texture>* textures;
        Ref<Texture> whiteTexture;
        Vertex* vertices;
        uint32_t* indices;
        uint32_t vertexCount;
        uint32_t indexCount;
        uint32_t textureCount;
        uint32_t reservedTexturesCount;
        Statistics statistics;

    public:
        explicit Renderer(ResourceLoader* resourceLoader);

        ~Renderer();

        [[nodiscard]] Statistics GetStatistics() const;

        void SetViewport(uint32_t width, uint32_t height, uint32_t x = 0, uint32_t y = 0) const;

        void BeginScene(OrthographicCamera* camera);

        void SubmitQuad(Quad& quad);

        void EndScene();

    private:
        void Reset();

        void DrawIndexed();

        void Flush();
    };
}

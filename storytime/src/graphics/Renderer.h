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
        Ref<Texture> Texture = nullptr;
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
        glm::vec2 Size = { 0.0f, 0.0f };
        glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        float RotationInDegrees = 0.0f;
    };

    class Renderer
    {
    private:
        struct Vertex
        {
            glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
            glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
            glm::vec2 TextureCoordinate = { 0.0f, 0.0f };
            float TextureIndex = 0.0f;
        };

        static const uint32_t VERTICES_PER_QUAD;
        static const uint32_t INDICES_PER_QUAD;
        static const uint32_t QUADS_PER_BATCH;
        static const uint32_t VERTICES_PER_BATCH;
        static const uint32_t INDICES_PER_BATCH;
        static const uint32_t MAX_TEXTURE_SLOTS;

        Ref<VertexArray> vertexArray = nullptr;
        Ref<VertexBuffer> vertexBuffer = nullptr;
        Ref<Shader> shader = nullptr;
        Ref<Texture>* textures = nullptr;
        Ref<Texture> whiteTexture = nullptr;
        Vertex* vertices = nullptr;
        uint32_t* indices = nullptr;
        glm::vec4* vertexPositionOriginOffset = nullptr;
        uint32_t vertexCount = 0;
        uint32_t indexCount = 0;
        uint32_t textureCount = 0;
        uint32_t whiteTextureIndex = 0;

    public:
        explicit Renderer(ResourceLoader* resourceLoader);

        ~Renderer();

        void SetViewport(uint32_t width, uint32_t height, uint32_t x = 0, uint32_t y = 0) const;

        void BeginScene(OrthographicCamera* camera);

        void DrawQuad(Quad& quad);

        void EndScene() const;

    private:
        void DrawIndexed() const;
    };
}

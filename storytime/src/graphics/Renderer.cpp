#include "system/Log.h"
#include "Renderer.h"

namespace storytime
{
    constexpr uint32_t Renderer::VERTICES_PER_QUAD = 4;
    constexpr uint32_t Renderer::INDICES_PER_QUAD = 6;
    constexpr uint32_t Renderer::QUADS_PER_BATCH = 1000;
    constexpr uint32_t Renderer::VERTICES_PER_BATCH = QUADS_PER_BATCH * VERTICES_PER_QUAD;
    constexpr uint32_t Renderer::INDICES_PER_BATCH = QUADS_PER_BATCH * INDICES_PER_QUAD;
    constexpr uint32_t Renderer::MAX_TEXTURE_SLOTS = 16;

    Renderer::Renderer(ResourceLoader* resourceLoader)
    {
        ST_GL_CALL(ST_TAG, glClearColor(0.1f, 0.1f, 0.1f, 1));

        uint32_t vertexBufferSize = sizeof(Vertex) * VERTICES_PER_BATCH;
        vertexBuffer = CreateRef<VertexBuffer>(vertexBufferSize);
        vertexBuffer->SetAttributeLayout({
            { GLSLType::Vec3,  "position" },
            { GLSLType::Vec4,  "color" },
            { GLSLType::Vec2,  "textureCoordinate" },
            { GLSLType::Float, "textureIndex" }
        });
        vertices = new Vertex[VERTICES_PER_BATCH];

        indices = new uint32_t[INDICES_PER_BATCH];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < INDICES_PER_BATCH; i += INDICES_PER_QUAD)
        {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            offset += VERTICES_PER_QUAD;
        }
        auto indexBuffer = CreateRef<IndexBuffer>(indices, INDICES_PER_BATCH);

        vertexArray = CreateRef<VertexArray>();
        vertexArray->AddVertexBuffer(vertexBuffer);
        vertexArray->SetIndexBuffer(indexBuffer);
        vertexArray->Bind();

        shader = resourceLoader->LoadShader("texture.vertex.glsl", "texture.fragment.glsl");
        shader->Bind();

        whiteTextureIndex = 0;
        whiteTexture = CreateRef<Texture>(1, 1);
        uint32_t whiteTexturePixels = 0xffffffff;
        whiteTexture->SetPixels(&whiteTexturePixels);

        textures = new Ref<Texture>[MAX_TEXTURE_SLOTS];
        textures[0] = whiteTexture;

        int32_t samplers[MAX_TEXTURE_SLOTS];
        for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
        {
            samplers[i] = i;
        }
        shader->SetIntArray("textureSamplers", samplers, MAX_TEXTURE_SLOTS);

        vertexPositionOriginOffset = new glm::vec4[4];
        vertexPositionOriginOffset[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
        vertexPositionOriginOffset[1] = glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
        vertexPositionOriginOffset[2] = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
        vertexPositionOriginOffset[3] = glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);

        ST_LOG_TRACE(ST_TAG, "Created");
    }

    Renderer::~Renderer()
    {
        shader->Unbind();
        vertexArray->Unbind();
        delete[] vertexPositionOriginOffset;
        delete[] textures;
        delete[] indices;
        delete[] vertices;
        ST_LOG_TRACE(ST_TAG, "Destroyed");
    }

    void Renderer::SetViewport(uint32_t width, uint32_t height, uint32_t x, uint32_t y) const
    {
        ST_GL_CALL(ST_TAG, glViewport(x, y, width, height));
    }

    void Renderer::BeginScene(OrthographicCamera* camera)
    {
        ST_GL_CALL(ST_TAG, glClear(GL_COLOR_BUFFER_BIT));
        shader->SetMat4("viewProjection", camera->GetViewProjection());
        vertexCount = 0;
        indexCount = 0;
        textureCount = whiteTextureIndex + 1;
    }

    void Renderer::DrawQuad(Quad& quad)
    {
        if (!quad.Texture)
        {
            quad.Texture = whiteTexture;
        }
        float textureSlot = -1.0f;
        for (uint32_t i = whiteTextureIndex + 1; i < textureCount; i++)
        {
            if (textures[i] == quad.Texture)
            {
                textureSlot = (float) i;
                break;
            }
        }
        if (textureSlot == -1.0f)
        {
            textureSlot = (float) textureCount;
            textures[textureCount] = quad.Texture;
            textureCount++;
        }

        const auto& translation = glm::translate(glm::mat4(1.0f), quad.Position);
        const auto& rotation = glm::rotate(glm::mat4(1.0f), glm::radians(quad.RotationInDegrees), { 0.0f, 0.0f, 1.0f });
        const auto& scale = glm::scale(glm::mat4(1.0f), { quad.Size.x, quad.Size.y, 1.0f });
        glm::mat4 transform = translation * rotation * scale;

        vertices[vertexCount].Position = transform * vertexPositionOriginOffset[0];
        vertices[vertexCount].Color = quad.Color;
        vertices[vertexCount].TextureCoordinate = { 0.0f, 0.0f };
        vertices[vertexCount].TextureIndex = textureSlot;
        vertexCount++;

        vertices[vertexCount].Position = transform * vertexPositionOriginOffset[1];
        vertices[vertexCount].Color = quad.Color;
        vertices[vertexCount].TextureCoordinate = { 1.0f, 0.0f };
        vertices[vertexCount].TextureIndex = textureSlot;
        vertexCount++;

        vertices[vertexCount].Position = transform * vertexPositionOriginOffset[2];
        vertices[vertexCount].Color = quad.Color;
        vertices[vertexCount].TextureCoordinate = { 1.0f, 1.0f };
        vertices[vertexCount].TextureIndex = textureSlot;
        vertexCount++;

        vertices[vertexCount].Position = transform * vertexPositionOriginOffset[3];
        vertices[vertexCount].Color = quad.Color;
        vertices[vertexCount].TextureCoordinate = { 0.0f, 1.0f };
        vertices[vertexCount].TextureIndex = textureSlot;
        vertexCount++;

        indexCount += INDICES_PER_QUAD;
    }

    void Renderer::EndScene() const
    {
        uint32_t verticesSize = vertexCount * sizeof(Vertex);
        vertexBuffer->SetVertices(vertices, verticesSize);
        for (uint32_t textureSlot = 0; textureSlot < textureCount; textureSlot++)
        {
            textures[textureSlot]->Bind(textureSlot);
        }
        DrawIndexed();
    }

    void Renderer::DrawIndexed() const
    {
        void* offset = nullptr;
        ST_GL_CALL(ST_TAG, glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, offset));
    }

}

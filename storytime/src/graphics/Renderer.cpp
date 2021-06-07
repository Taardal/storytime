#include "system/Log.h"
#include "Renderer.h"

namespace storytime
{
    Quad::Quad()
            : Texture(nullptr),
              Position(0.0f, 0.0f, 0.0f),
              Size(0.0f, 0.0f),
              Color(1.0f, 1.0f, 1.0f, 1.0f),
              RotationInDegrees(0.0f),
              TilingFactor(1.0f)
    {}
}

namespace storytime
{
    Renderer::Statistics::Statistics()
            : DrawCalls(0),
              QuadCount(0)
    {}

    uint32_t Renderer::Statistics::GetVertexCount() const
    {
        return QuadCount * VERTICES_PER_QUAD;
    }

    uint32_t Renderer::Statistics::GetIndexCount() const
    {
        return QuadCount * INDICES_PER_QUAD;
    }
}

namespace storytime
{
    Renderer::Vertex::Vertex()
            : Position(0.0f, 0.0f, 0.0f),
              Color(1.0f, 1.0f, 1.0f, 1.0f),
              TextureCoordinate(0.0f, 0.0f),
              TextureIndex(0.0f),
              TilingFactor(1.0f)
    {}
}

namespace storytime
{
    constexpr uint32_t Renderer::VERTICES_PER_QUAD = 4;
    constexpr uint32_t Renderer::INDICES_PER_QUAD = 6;
    constexpr uint32_t Renderer::QUADS_PER_BATCH = 1000;
    constexpr uint32_t Renderer::VERTICES_PER_BATCH = QUADS_PER_BATCH * VERTICES_PER_QUAD;
    constexpr uint32_t Renderer::INDICES_PER_BATCH = QUADS_PER_BATCH * INDICES_PER_QUAD;
    constexpr uint32_t Renderer::MAX_TEXTURE_SLOTS = 16;

    Renderer::Renderer(ResourceLoader* resourceLoader, CoordinateSystem coordinateSystem)
            : vertexArray(CreateRef<VertexArray>()),
              vertexBuffer(CreateRef<VertexBuffer>(sizeof(Vertex) * VERTICES_PER_BATCH)),
              shader(resourceLoader->LoadShader("res/shaders/texture.vertex.glsl", "res/shaders/texture.fragment.glsl")),
              textures(new Ref<Texture>[MAX_TEXTURE_SLOTS]),
              whiteTexture(CreateRef<Texture>(1, 1)),
              vertices(new Vertex[VERTICES_PER_BATCH]),
              vertexPositions(new glm::vec4[4]),
              vertexCount(0),
              indices(new uint32_t[INDICES_PER_BATCH]),
              indexCount(0),
              textureCount(0),
              reservedTexturesCount(0),
              statistics()
    {
        ST_GL_CALL(ST_TAG, glEnable(GL_BLEND));
        ST_GL_CALL(ST_TAG, glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        ST_GL_CALL(ST_TAG, glClearColor(1.0f, 0.0f, 1.0f, 1));

        vertexBuffer->SetAttributeLayout({
            { GLSLType::Vec3, "position" },
            { GLSLType::Vec4, "color" },
            { GLSLType::Vec2, "textureCoordinate" },
            { GLSLType::Float, "textureIndex" },
            { GLSLType::Float, "tilingFactor" }
        });

        VertexTransform vertexTransform = GetVertexTransform(coordinateSystem);
        if (vertexTransform == VertexTransform::RightUp)
        {
            vertexPositions[0] = { 0.0f, 0.0f, 0.0f, 1.0f };
            vertexPositions[1] = { 1.0f, 0.0f, 0.0f, 1.0f };
            vertexPositions[2] = { 1.0f, 1.0f, 0.0f, 1.0f };
            vertexPositions[3] = { 0.0f, 1.0f, 0.0f, 1.0f };
        }
        if (vertexTransform == VertexTransform::RightDown)
        {
            vertexPositions[0] = { 0.0f, 0.0f, 0.0f, 1.0f };
            vertexPositions[1] = { 1.0f, 0.0f, 0.0f, 1.0f };
            vertexPositions[2] = { 1.0f, -1.0f, 0.0f, 1.0f };
            vertexPositions[3] = { 0.0f, -1.0f, 0.0f, 1.0f };
        }
        if (vertexTransform == VertexTransform::Center)
        {
            vertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
            vertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
            vertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
            vertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
        }

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

        vertexArray->AddVertexBuffer(vertexBuffer);
        vertexArray->SetIndexBuffer(indexBuffer);
        vertexArray->Bind();

        uint32_t whiteTexturePixels = 0xffffffff;
        whiteTexture->SetPixels(&whiteTexturePixels);
        textures[0] = whiteTexture;
        textureCount++;
        reservedTexturesCount++;

        int32_t samplers[MAX_TEXTURE_SLOTS];
        for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
        {
            samplers[i] = i;
        }
        shader->Bind();
        shader->SetIntArray("textureSamplers", samplers, MAX_TEXTURE_SLOTS);
    }

    Renderer::~Renderer()
    {
        shader->Unbind();
        vertexArray->Unbind();
        delete[] textures;
        delete[] indices;
        delete[] vertices;
    }

    Renderer::Statistics Renderer::GetStatistics() const
    {
        return statistics;
    }

    void Renderer::SetViewport(uint32_t width, uint32_t height, uint32_t x, uint32_t y) const
    {
        ST_GL_CALL(ST_TAG, glViewport(x, y, width, height));
    }

    void Renderer::BeginFrame(const glm::mat4& viewProjection)
    {
        Reset();
        ST_GL_CALL(ST_TAG, glClear(GL_COLOR_BUFFER_BIT));
        shader->SetMat4("viewProjection", viewProjection);
    }

    void Renderer::EndFrame()
    {
        Flush();
    }

    void Renderer::SubmitQuad(Quad& quad)
    {
        if (indexCount >= INDICES_PER_BATCH)
        {
            Flush();
            Reset();
        }

        if (!quad.Texture)
        {
            quad.Texture = whiteTexture;
        }
        float textureIndex = -1.0f;
        for (uint32_t i = 0; i < textureCount; i++)
        {
            if (textures[i] == quad.Texture)
            {
                textureIndex = (float) i;
                break;
            }
        }
        if (textureIndex == -1.0f)
        {
            textureIndex = (float) textureCount;
            textures[textureCount] = quad.Texture;
            textureCount++;
        }

        const auto& translation = glm::translate(glm::mat4(1.0f), quad.Position);
        const auto& rotation = glm::rotate(glm::mat4(1.0f), glm::radians(quad.RotationInDegrees), { 0.0f, 0.0f, 1.0f });
        const auto& scale = glm::scale(glm::mat4(1.0f), { quad.Size.x, quad.Size.y, 1.0f });
        glm::mat4 transform = translation * rotation * scale;

        vertices[vertexCount].Position = transform * vertexPositions[0];
        vertices[vertexCount].Color = quad.Color;
        vertices[vertexCount].TextureCoordinate = { 0.0f, 0.0f };
        vertices[vertexCount].TextureIndex = textureIndex;
        vertices[vertexCount].TilingFactor = quad.TilingFactor;
        vertexCount++;

        vertices[vertexCount].Position = transform * vertexPositions[1];
        vertices[vertexCount].Color = quad.Color;
        vertices[vertexCount].TextureCoordinate = { 1.0f, 0.0f };
        vertices[vertexCount].TextureIndex = textureIndex;
        vertices[vertexCount].TilingFactor = quad.TilingFactor;
        vertexCount++;

        vertices[vertexCount].Position = transform * vertexPositions[2];
        vertices[vertexCount].Color = quad.Color;
        vertices[vertexCount].TextureCoordinate = { 1.0f, 1.0f };
        vertices[vertexCount].TextureIndex = textureIndex;
        vertices[vertexCount].TilingFactor = quad.TilingFactor;
        vertexCount++;

        vertices[vertexCount].Position = transform * vertexPositions[3];
        vertices[vertexCount].Color = quad.Color;
        vertices[vertexCount].TextureCoordinate = { 0.0f, 1.0f };
        vertices[vertexCount].TextureIndex = textureIndex;
        vertices[vertexCount].TilingFactor = quad.TilingFactor;
        vertexCount++;

        indexCount += INDICES_PER_QUAD;
        statistics.QuadCount++;
    }

    void Renderer::SubmitQuadFoo(Quad& quad, const glm::vec2* textureCoordinates)
    {
        if (indexCount >= INDICES_PER_BATCH)
        {
            Flush();
            Reset();
        }
        if (!quad.Texture)
        {
            quad.Texture = whiteTexture;
        }
        float textureIndex = -1.0f;
        for (uint32_t i = 0; i < textureCount; i++)
        {
            if (textures[i] == quad.Texture)
            {
                textureIndex = (float) i;
                break;
            }
        }
        if (textureIndex == -1.0f)
        {
            textureIndex = (float) textureCount;
            textures[textureCount] = quad.Texture;
            textureCount++;
        }
        const glm::mat4& translation = glm::translate(glm::mat4(1.0f), quad.Position);
        const glm::mat4& rotation = glm::rotate(glm::mat4(1.0f), glm::radians(quad.RotationInDegrees), { 0.0f, 0.0f, 1.0f });
        const glm::mat4& scale = glm::scale(glm::mat4(1.0f), { quad.Size.x, quad.Size.y, 1.0f });
        const glm::mat4& transform = translation * rotation * scale;
        for (int i = 0; i < VERTICES_PER_QUAD; i++)
        {
            vertices[vertexCount].Position = transform * vertexPositions[i];
            vertices[vertexCount].Color = quad.Color;
            vertices[vertexCount].TextureCoordinate = textureCoordinates[i];
            vertices[vertexCount].TextureIndex = textureIndex;
            vertices[vertexCount].TilingFactor = quad.TilingFactor;
            vertexCount++;
            indexCount += INDICES_PER_QUAD;
            statistics.QuadCount++;
        }
    }

    /*
        vertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        vertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        vertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        vertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
    */

    void Renderer::Reset()
    {
        vertexCount = 0;
        indexCount = 0;
        textureCount = reservedTexturesCount;
        memset(&statistics, 0, sizeof(Statistics));
    }

    void Renderer::Flush()
    {
        vertexBuffer->SetVertices(vertices, vertexCount * sizeof(Vertex));
        for (uint32_t i = 0; i < textureCount; i++)
        {
            textures[i]->Bind(i);
        }
        DrawIndexed();
    }

    void Renderer::DrawIndexed()
    {
        void* offset = nullptr;
        ST_GL_CALL(ST_TAG, glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, offset));
        statistics.DrawCalls++;
    }

    Renderer::VertexTransform Renderer::GetVertexTransform(CoordinateSystem coordinateSystem)
    {
        switch (coordinateSystem)
        {
            case CoordinateSystem::RightUp:
                return VertexTransform::RightUp;
            case CoordinateSystem::RightDown:
                return VertexTransform::RightDown;
            default:
                return VertexTransform::Center;
        }
    }

}

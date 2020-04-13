#include "system/Log.h"
#include "Renderer.h"

namespace storytime
{
    constexpr uint32_t Renderer::VERTICES_PER_QUAD = 4;
    constexpr uint32_t Renderer::INDICES_PER_QUAD = 6;
    constexpr uint32_t Renderer::QUADS_PER_BATCH = 1000;
    constexpr uint32_t Renderer::VERTICES_PER_BATCH = QUADS_PER_BATCH * VERTICES_PER_QUAD;
    constexpr uint32_t Renderer::INDICES_PER_BATCH = QUADS_PER_BATCH * INDICES_PER_QUAD;

    Renderer::Renderer(ResourceLoader* resourceLoader)
            : vertices(new Vertex[VERTICES_PER_BATCH]), indices(new uint32_t[INDICES_PER_BATCH]), vertexCount(0), indexCount(0)
    {
        ST_TRACE(ST_TAG, "Creating");

        uint32_t vertexBufferSize = sizeof(Vertex) * VERTICES_PER_BATCH;
        vertexBuffer = CreateRef<VertexBuffer>(vertexBufferSize);
        vertexBuffer->setAttributeLayout({
            { GLSLType::Vec3, "position" },
            { GLSLType::Vec4, "color" }
        });

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
        vertexArray->addVertexBuffer(vertexBuffer);
        vertexArray->setIndexBuffer(indexBuffer);
        vertexArray->bind();

        shader = resourceLoader->LoadShader("flat_color.vertex.glsl", "flat_color.fragment.glsl");
        shader->bind();

        glClearColor(0.1f, 0.1f, 0.1f, 1);

        ST_TRACE(ST_TAG, "Created");
    }

    Renderer::~Renderer()
    {
        ST_TRACE(ST_TAG, "Destroying");
        shader->unbind();
        vertexArray->unbind();
        delete[] indices;
        delete[] vertices;
        ST_TRACE(ST_TAG, "Destroyed");
    }

    void Renderer::SetViewport(uint32_t width, uint32_t height, uint32_t x, uint32_t y) const
    {
        glViewport(x, y, width, height);
    }

    void Renderer::BeginScene(OrthographicCamera* camera)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        shader->setMat4("viewProjection", camera->getViewProjection());
        vertexCount = 0;
        indexCount = 0;
    }

    void Renderer::DrawQuad(const Quad& quad)
    {
        const auto& translation = glm::translate(glm::mat4(1.0f), quad.Position);
        const auto& rotation = glm::rotate(glm::mat4(1.0f), glm::radians(quad.RotationInDegrees), { 0.0f, 0.0f, 1.0f });
        const auto& scale = glm::scale(glm::mat4(1.0f), { quad.Size.x, quad.Size.y, 1.0f });
        glm::mat4 transform = translation * rotation * scale;

        vertices[vertexCount].Position = transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
        vertices[vertexCount].Color = quad.Color;
        vertexCount++;

        vertices[vertexCount].Position = transform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
        vertices[vertexCount].Color = quad.Color;
        vertexCount++;

        vertices[vertexCount].Position = transform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
        vertices[vertexCount].Color = quad.Color;
        vertexCount++;

        vertices[vertexCount].Position = transform * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
        vertices[vertexCount].Color = quad.Color;
        vertexCount++;

        indexCount += INDICES_PER_QUAD;
    }

    void Renderer::EndScene() const
    {
        uint32_t verticesSize = vertexCount * sizeof(Vertex);
        vertexBuffer->SetVertices(vertices, verticesSize);
        DrawIndexed();
    }

    void Renderer::DrawIndexed() const
    {
        void* offset = nullptr;
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, offset);
    }


}

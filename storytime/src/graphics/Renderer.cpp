#include "system/Log.h"
#include "Renderer.h"

namespace storytime {

    Renderer::Renderer() {
        ST_TRACE(ST_TAG, "Creating");

        float vertices[] = {
                -0.5f, -0.5f, 0.0f,
                 0.5f, -0.5f, 0.0f,
                 0.5f,  0.5f, 0.0f,
                -0.5f,  0.5f, 0.0f,
        };
        auto vertexBuffer = CreateRef<VertexBuffer>(vertices, sizeof(vertices));
        vertexBuffer->setAttributeLayout({{ GLSLType::Vec3, "in_position" }});

        unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
        unsigned int numberOfIndices = sizeof(indices) / sizeof(unsigned int);
        auto indexBuffer = CreateRef<IndexBuffer>(indices, numberOfIndices);

        vertexArray = CreateRef<VertexArray>();
        vertexArray->addVertexBuffer(vertexBuffer);
        vertexArray->setIndexBuffer(indexBuffer);
        vertexArray->bind();

        std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 position;

            uniform mat4 viewProjection;
            uniform mat4 transform;

			void main() {
				gl_Position = viewProjection * transform * vec4(position, 1.0);
			}
		)";
        std::string fragmentSource = R"(
			#version 330 core

            layout(location = 0) out vec4 colorOutput;

			uniform vec4 color;

			void main() {
				colorOutput = color;
			}
		)";
        shader = CreateRef<Shader>(vertexSource, fragmentSource);
        shader->bind();

        glClearColor(0.1f, 0.1f, 0.1f, 1);

        ST_TRACE(ST_TAG, "Created");
    }

    Renderer::~Renderer() {
        ST_TRACE(ST_TAG, "Destroying");
        vertexArray->unbind();
        shader->unbind();
        ST_TRACE(ST_TAG, "Destroyed");
    }

    void Renderer::beginScene(OrthographicCamera* camera) const {
        glClear(GL_COLOR_BUFFER_BIT);
        shader->setMat4("viewProjection", camera->getViewProjection());
    }

    void Renderer::drawElements() const {
        void* offset = nullptr;
        glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, offset);
    }

    void Renderer::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
        shader->setFloat4("color", color);
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        shader->setMat4("transform", transform);
        drawElements();
    }

}

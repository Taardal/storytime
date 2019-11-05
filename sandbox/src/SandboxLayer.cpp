#include "SandboxLayer.h"

namespace Sandbox {

    SandboxLayer::SandboxLayer(std::string_view name) : Layer(name) {
        LOG_TRACE(TAG, "Creating");
        float vertices[] = {
                -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
                0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };
        vertexBuffer = Darkle::CreateRef<Darkle::VertexBuffer>(vertices, sizeof(vertices));
        vertexBuffer->setAttributeLayout({
            { Darkle::GLSLType::Vec3, "in_position" },
            { Darkle::GLSLType::Vec4, "in_color" }
        });

        unsigned int numberOfIndices = 3;
        unsigned int indices[] = {0, 1, 2};
        indexBuffer = Darkle::CreateRef<Darkle::IndexBuffer>(indices, numberOfIndices);

        vertexArray = Darkle::CreateRef<Darkle::VertexArray>();
        vertexArray->addVertexBuffer(vertexBuffer);
        vertexArray->setIndexBuffer(indexBuffer);

        std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 in_position;
			layout(location = 1) in vec4 in_color;

			out Vertex {
				vec4 color;
			} out_vertex;

			void main()
			{
				out_vertex.color = in_color;
				gl_Position = vec4(in_position, 1.0);
			}
		)";
        std::string fragmentSource = R"(
			#version 330 core

			in Vertex {
				vec4 color;
			} in_vertex;

			out vec4 out_color;

			void main()
			{
				out_color = in_vertex.color;
			}
		)";
        shader = Darkle::CreateRef<Darkle::Shader>(vertexSource, fragmentSource);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        LOG_TRACE(TAG, "Created");
    }

    SandboxLayer::~SandboxLayer() {
        LOG_TRACE(TAG, "Destroyed");
    }

    void SandboxLayer::onAttach() {
        vertexArray->bind();
        vertexBuffer->bind();
        indexBuffer->bind();
        shader->bind();
    }

    void SandboxLayer::onDetach() {
        vertexArray->unbind();
        vertexBuffer->unbind();
        indexBuffer->unbind();
        shader->unbind();
    }

    void SandboxLayer::onUpdate(Darkle::Renderer* renderer) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
    }

    void SandboxLayer::onEvent(const Darkle::Event& event) {

    }

}


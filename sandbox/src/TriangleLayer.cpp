#include "TriangleLayer.h"

namespace sandbox {

    TriangleLayer::TriangleLayer() : Layer("TriangleLayer") {
        ST_TRACE(ST_TAG, "Creating");

        float vertices[] = {
                -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
                0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };
        auto vertexBuffer = storytime::CreateRef<storytime::VertexBuffer>(vertices, sizeof(vertices));
        vertexBuffer->setAttributeLayout({
            { storytime::GLSLType::Vec3, "in_position" },
            { storytime::GLSLType::Vec4, "in_color" }
        });

        unsigned int numberOfIndices = 3;
        unsigned int indices[] = {0, 1, 2};
        auto indexBuffer = storytime::CreateRef<storytime::IndexBuffer>(indices, numberOfIndices);

        vertexArray = storytime::CreateRef<storytime::VertexArray>();
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
        shader = storytime::CreateRef<storytime::Shader>(vertexSource, fragmentSource);

        ST_TRACE(ST_TAG, "Created");
    }

    TriangleLayer::~TriangleLayer() {
        ST_TRACE(ST_TAG, "Destroyed");
    }

    void TriangleLayer::onAttach() {
        vertexArray->bind();
        shader->bind();
    }

    void TriangleLayer::onDetach() {
        vertexArray->unbind();
        shader->unbind();
    }

    void TriangleLayer::onUpdate(storytime::Renderer* renderer, storytime::Timestep timestep) {
        renderer->drawElements(vertexArray);
    }

    void TriangleLayer::onEvent(const storytime::Event& event) {

    }

}


#include "Log.h"
#include "Renderer.h"
#include <glad/glad.h>
#include <iostream>

namespace Darkle {

    Renderer::Renderer() : vertexArray(nullptr), vertexBuffer(nullptr), indexBuffer(nullptr) {
        LOG_TRACE(TAG, "Creating");
        float vertices[] = {
                -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
                0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };
        vertexBuffer = new VertexBuffer(vertices, sizeof(vertices), {
                { GLSLType::Vec3, "in_position" },
                { GLSLType::Vec4, "in_color" }
        });

        unsigned int indices[] = { 0, 1, 2 };
        indexBuffer = new IndexBuffer(indices, 3);

        vertexArray = new VertexArray();
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
        shader = new Shader(vertexSource, fragmentSource);
        shader->Bind();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        LOG_TRACE(TAG, "Created");
    }

    Renderer::~Renderer() {
        LOG_TRACE(TAG, "Destroying");
        shader->Unbind();
        vertexArray->unbind();
        delete shader;
        delete vertexArray;
        LOG_TRACE(TAG, "Destroyed");
    }


    void Renderer::render() {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

        while (GLenum error = glGetError()) {
            std::cout << "GL error [" << error << "]" << std::endl;
        }
    }

}

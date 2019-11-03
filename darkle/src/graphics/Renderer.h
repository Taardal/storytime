#pragma once

#include "VertexArray.h"
#include "Shader.h"

namespace Darkle {

    class Renderer {
    private:
        VertexArray* vertexArray;
        VertexBuffer* vertexBuffer;
        IndexBuffer* indexBuffer;
        Shader* shader;
    public:
        Renderer();

        ~Renderer();

        void render();
    };

}



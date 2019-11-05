#pragma once

#include "Core.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Darkle {

    class Renderer {
    private:
        //Ref<VertexArray> vertexArray;
        //Ref<VertexBuffer> vertexBuffer;
        //Ref<IndexBuffer> indexBuffer;
        //Ref<Shader> shader;
    public:
        Renderer();

        ~Renderer();

        void render();
    };

}



#pragma once

#include "system/Core.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Darkle {

    class Renderer {
    public:
        Renderer();

        ~Renderer();

        void beginScene() const;

        void drawElements(const Ref<IndexBuffer>& indexBuffer) const;
    };

}

#include "system/Log.h"
#include "Renderer.h"

namespace storytime {

    Renderer::Renderer() {
        ST_TRACE(ST_TAG, "Created");
    }

    Renderer::~Renderer() {
        ST_TRACE(ST_TAG, "Destroyed");
    }

    void Renderer::beginScene() const {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::drawElements(const Ref<VertexArray>& vertexArray) const {
        void* offset = nullptr;
        glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, offset);
    }

}

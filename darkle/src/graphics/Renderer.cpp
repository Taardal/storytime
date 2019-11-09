#include "system/Log.h"
#include "Renderer.h"

namespace Darkle {

    Renderer::Renderer() {
        LOG_TRACE(TAG, "Created");
    }

    Renderer::~Renderer() {
        LOG_TRACE(TAG, "Destroyed");
    }

    void Renderer::beginScene() const {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::drawElements(const Ref<IndexBuffer>& indexBuffer) const {
        void* offset = nullptr;
        glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, offset);
    }

}

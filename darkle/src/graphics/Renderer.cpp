#include "Log.h"
#include "Renderer.h"
#include <glad/glad.h>
#include <iostream>

namespace Darkle {

    Renderer::Renderer() {
        LOG_TRACE(TAG, "Created");
    }

    Renderer::~Renderer() {
        LOG_TRACE(TAG, "Destroyed");
    }


    void Renderer::render() {

    }

}

#pragma once

#include "GraphicsContext.h"
#include <imgui.h>

namespace storytime
{
    class ImGuiRenderer
    {
    private:
        GraphicsContext* graphicsContext;

    public:
        explicit ImGuiRenderer(GraphicsContext* graphicsContext);

        virtual ~ImGuiRenderer();

        void Init(GLFWwindow* glfwWindow) const;

        void BeginScene() const;

        void EndScene() const;
    };
}



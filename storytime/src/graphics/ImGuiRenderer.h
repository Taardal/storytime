#pragma once

#include "GraphicsContext.h"
#include "window/window.h"
#include "window/Event.h"
#include <imgui.h>

namespace Storytime
{
    class ImGuiRenderer
    {
    private:
        GraphicsContext* graphicsContext;
        Window* window = nullptr;
        bool consumeEvents;

    public:
        explicit ImGuiRenderer(GraphicsContext* graphicsContext);

        ImGuiRenderer(GraphicsContext* graphicsContext, Window* window);

        virtual ~ImGuiRenderer();

        void Init(GLFWwindow* glfwWindow) const;

        void Begin() const;

        void End(float windowWidth, float windowHeight) const;

        void SetConsumeEvents(bool consumeEvents);

        void OnEvent(Event& event) const;

    };
}



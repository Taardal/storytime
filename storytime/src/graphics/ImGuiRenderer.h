#pragma once

#include "GraphicsContext.h"
#include "window/events/Event.h"
#include <imgui.h>

namespace Storytime
{
    class ImGuiRenderer
    {
    private:
        GraphicsContext* graphicsContext;
        bool consumeEvents;

    public:
        explicit ImGuiRenderer(GraphicsContext* graphicsContext);

        virtual ~ImGuiRenderer();

        void Init(GLFWwindow* glfwWindow) const;

        void Begin() const;

        void End(float windowWidth, float windowHeight) const;

        void SetConsumeEvents(bool consumeEvents);

        void OnEvent(Event& event) const;

    };
}



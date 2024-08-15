#pragma once

#include "graphics/Renderer.h"
#include "graphics/ImGuiRenderer.h"
#include "window/Input.h"
#include "Event.h"

namespace Storytime
{
    class Layer
    {
    private:
        std::string_view name;

    public:
        explicit Layer(std::string_view name);

        virtual ~Layer() = default;

        [[nodiscard]] std::string_view GetName() const;

        [[nodiscard]] std::string ToString() const;

        virtual void OnAttach() = 0;

        virtual void OnEvent(const Event& event) = 0;

        virtual void OnUpdate(f32 timestep, Input* input, Renderer* renderer) = 0;

        virtual void OnImGuiRender(ImGuiRenderer* imGuiRenderer) = 0;

        virtual void OnDetach() = 0;
    };

}



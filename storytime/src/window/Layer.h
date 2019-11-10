#pragma once

#include "graphics/Renderer.h"
#include "system/Timestep.h"
#include "window/Input.h"
#include "window/events/Event.h"

namespace storytime {

    class Layer {
    private:
        std::string_view name;

    public:
        explicit Layer(std::string_view name);

        virtual ~Layer() = default;

        [[nodiscard]] std::string_view getName() const;

        [[nodiscard]] std::string toString() const;

        virtual void onAttach() = 0;

        virtual void onDetach() = 0;

        virtual void onUpdate(Timestep timestep, Renderer* renderer, Input* input) = 0;

        virtual void onEvent(const Event& event) = 0;
    };

}



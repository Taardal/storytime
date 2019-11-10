#pragma once

#include "storytime/Storytime.h"

namespace sandbox {

    class TriangleLayer : public storytime::Layer {
    private:
        storytime::Ref<storytime::VertexArray> vertexArray;
        storytime::Ref<storytime::Shader> shader;

    public:
        TriangleLayer();

        ~TriangleLayer() override;

    private:
        void onAttach() override;

        void onDetach() override;

        void onUpdate(storytime::Timestep timestep, storytime::Renderer* renderer) override;

        void onEvent(const storytime::Event& event) override;
    };

}



#pragma once

#include "storytime/Storytime.h"

namespace sandbox {

    class TriangleLayer : public storytime::Layer {
    private:
        storytime::Ref<storytime::VertexArray> vertexArray;
        storytime::Ref<storytime::Shader> shader;
        storytime::OrthographicCameraController cameraController;

    public:
        explicit TriangleLayer(storytime::OrthographicCameraController cameraController);

        ~TriangleLayer() override;

    private:
        void onAttach() override;

        void onDetach() override;

        void onUpdate(storytime::Timestep timestep, storytime::Renderer* renderer, storytime::Input* input) override;

        void onEvent(const storytime::Event& event) override;
    };

}



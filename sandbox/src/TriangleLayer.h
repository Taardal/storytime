#pragma once

#include "storytime/Storytime.h"

namespace sandbox {

    class TriangleLayer : public st::Layer {
    private:
        st::OrthographicCameraController* cameraController;

    public:
        explicit TriangleLayer(st::OrthographicCameraController* cameraController);

        ~TriangleLayer() override;

    private:
        void onAttach() override;

        void onDetach() override;

        void onUpdate(st::Timestep timestep, st::Renderer* renderer, st::Input* input) override;

        void onEvent(const st::Event& event) override;
    };

}



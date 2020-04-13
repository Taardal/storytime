#pragma once

#include "storytime/Storytime.h"

namespace sandbox
{
    class TriangleLayer : public st::Layer
    {
    private:
        st::OrthographicCameraController* cameraController;

    public:
        explicit TriangleLayer(st::OrthographicCameraController* cameraController);

        ~TriangleLayer() override;

    private:
        void OnAttach() override;

        void OnDetach() override;

        void OnUpdate(st::Timestep timestep, st::Renderer* renderer, st::Input* input) override;

        void OnEvent(const st::Event& event) override;
    };

}



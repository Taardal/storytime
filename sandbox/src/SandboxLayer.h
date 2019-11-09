#pragma once

#include <system/Timestep.h>
#include "darkle/Darkle.h"

namespace Sandbox {

    class SandboxLayer : public Darkle::Layer {
    private:
        Darkle::Ref<Darkle::VertexArray> vertexArray;
        Darkle::Ref<Darkle::VertexBuffer> vertexBuffer;
        Darkle::Ref<Darkle::IndexBuffer> indexBuffer;
        Darkle::Ref<Darkle::Shader> shader;

    public:
        explicit SandboxLayer(std::string_view name);

        ~SandboxLayer() override;

    private:
        void onAttach() override;

        void onDetach() override;

        void onUpdate(Darkle::Renderer* renderer, Darkle::Timestep timestep) override;

        void onEvent(const Darkle::Event& event) override;
    };

}



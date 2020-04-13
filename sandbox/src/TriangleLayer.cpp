#include "system/Log.h"
#include "TriangleLayer.h"

namespace sandbox
{
    TriangleLayer::TriangleLayer(st::OrthographicCameraController* cameraController)
            : Layer("TriangleLayer"), cameraController(cameraController)
    {
        ST_TRACE(ST_TAG, "Created");
    }

    TriangleLayer::~TriangleLayer()
    {
        ST_TRACE(ST_TAG, "Destroyed");
    }

    void TriangleLayer::OnAttach()
    {
    }

    void TriangleLayer::OnDetach()
    {
    }

    void TriangleLayer::OnUpdate(st::Timestep timestep, st::Renderer* renderer, st::Input* input)
    {
        cameraController->onUpdate(timestep, input);

        static float rotation = 0.0f;
        rotation += timestep * 50.0f;

        for (uint32_t x = 0; x < 5; x++)
        {
            for (uint32_t y = 0; y < 5; y++)
            {
                st::Quad quad{};
                quad.Position = { x, y, 0.0f };
                quad.Size = { 1.0f, 1.0f };
                quad.Color = { (x + y) % 2, 0.2f, 0.5f, 1.0f };
                quad.RotationInDegrees = rotation;
                renderer->DrawQuad(quad);
            }
        }
    }

    void TriangleLayer::OnEvent(const st::Event& event)
    {
        cameraController->onEvent(event);
    }

}


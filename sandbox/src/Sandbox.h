#pragma once

#include "storytime/Storytime.h"

class Sandbox : public st::Application
{
public:
    Sandbox(st::Window* window, st::Renderer* renderer, st::ImGuiRenderer* imGuiRenderer, st::Input* input, st::OrthographicCameraController* cameraController, st::ResourceLoader* resourceLoader);
};


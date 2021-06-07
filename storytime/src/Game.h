#pragma once

#include "system/Timestep.h"
#include "window/events/Event.h"

namespace storytime
{
    class Game
    {
    public:
        virtual ~Game() {};

        virtual void OnUpdate(Timestep timestep) = 0;

        virtual void OnRender() = 0;

        virtual void OnEvent(const Event& event) = 0;
    };
}
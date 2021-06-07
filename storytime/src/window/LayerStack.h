#pragma once

#include "Layer.h"
#include <vector>

namespace storytime
{
    class LayerStack
    {
    private:
        std::vector<Layer*> layers;
        uint32_t insertIndex;

    public:
        LayerStack();

        ~LayerStack();

        void PushLayer(Layer* layer);

        void PopLayer(Layer* layer);

        std::vector<Layer*>::const_iterator begin() const;

        std::vector<Layer*>::const_iterator end() const;

        std::vector<Layer*>::const_reverse_iterator rbegin() const;

        std::vector<Layer*>::const_reverse_iterator rend() const;

    };

}



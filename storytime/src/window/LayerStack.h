#pragma once

#include "Layer.h"
#include <vector>

namespace Storytime
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

        [[nodiscard]] std::vector<Layer*>::const_iterator begin() const;

        [[nodiscard]] std::vector<Layer*>::const_iterator end() const;

        [[nodiscard]] std::vector<Layer*>::const_reverse_iterator rbegin() const;

        [[nodiscard]] std::vector<Layer*>::const_reverse_iterator rend() const;

    };

}



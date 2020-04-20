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

        [[nodiscard]] std::vector<Layer*> GetLayers() const;

        [[nodiscard]] std::vector<Layer*>::const_iterator begin() const;

        [[nodiscard]] std::vector<Layer*>::const_iterator end() const;

        void PushLayer(Layer* layer);

        void PopLayer(Layer* layer);
    };

}



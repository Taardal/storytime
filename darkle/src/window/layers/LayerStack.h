#pragma once

#include "Layer.h"
#include <vector>

namespace Darkle {

    class LayerStack {
    private:
        std::vector<Layer*> layers;
        unsigned int insertIndex;

    public:
        LayerStack();

        ~LayerStack();

        [[nodiscard]] std::vector<Layer*> getLayers() const;

        std::vector<Layer*>::iterator begin();

        std::vector<Layer*>::iterator end();

        void pushLayer(Layer* layer);

        void popLayer(Layer* layer);
    };

}



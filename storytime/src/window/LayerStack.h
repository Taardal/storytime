#pragma once

#include "Layer.h"
#include <vector>

namespace storytime {

    class LayerStack {
    private:
        std::vector<Layer*> layers;
        unsigned int insertIndex;

    public:
        LayerStack();

        ~LayerStack();

        [[nodiscard]] std::vector<Layer*> getLayers() const;

        [[nodiscard]] std::vector<Layer*>::const_iterator begin() const;

        [[nodiscard]] std::vector<Layer*>::const_iterator end() const;

        void pushLayer(Layer* layer);

        void popLayer(Layer* layer);
    };

}



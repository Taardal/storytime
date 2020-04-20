#include "system/Log.h"
#include "LayerStack.h"

namespace storytime {

    LayerStack::LayerStack() : layers{}, insertIndex(0) {
        ST_LOG_TRACE(ST_TAG, "Created");
    }

    LayerStack::~LayerStack() {
        ST_LOG_TRACE(ST_TAG, "Destroying");
        for (Layer* layer : layers) {
            layer->OnDetach();
            delete layer;
        }
        ST_LOG_TRACE(ST_TAG, "Destroyed");
    }

    std::vector<Layer*> LayerStack::getLayers() const {
        return layers;
    }

    std::vector<Layer*>::const_iterator LayerStack::begin() const {
        return layers.begin();
    }

    std::vector<Layer*>::const_iterator LayerStack::end() const {
        return layers.end();
    }

    void LayerStack::pushLayer(Layer* layer) {
        layers.emplace(layers.begin() + insertIndex, layer);
        insertIndex++;
        layer->OnAttach();
        ST_LOG_INFO(ST_TAG, "Pushed layer [{0}]", layer->getName());
    }

    void LayerStack::popLayer(Layer* layer) {
        auto iterator = std::find(layers.begin(), layers.begin() + insertIndex, layer);
        if (iterator != layers.end()) {
            layer->OnDetach();
            delete layer;
            layers.erase(iterator);
            insertIndex--;
        }
        ST_LOG_INFO(ST_TAG, "Popped layer [{0}]", layer->getName());
    }

}
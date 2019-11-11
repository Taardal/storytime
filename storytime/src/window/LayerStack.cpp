#include "system/Log.h"
#include "LayerStack.h"

namespace storytime {

    LayerStack::LayerStack() : layers{}, insertIndex(0) {
        ST_TRACE(ST_TAG, "Created");
    }

    LayerStack::~LayerStack() {
        ST_TRACE(ST_TAG, "Destroying");
        for (Layer* layer : layers) {
            layer->onDetach();
            delete layer;
        }
        ST_TRACE(ST_TAG, "Destroyed");
    }

    std::vector<Layer*> LayerStack::getLayers() const {
        return layers;
    }

    std::vector<Layer*>::iterator LayerStack::begin() {
        return layers.begin();
    }

    std::vector<Layer*>::iterator LayerStack::end() {
        return layers.end();
    }

    void LayerStack::pushLayer(Layer* layer) {
        ST_DEBUG(ST_TAG, "Pushing layer [{0}]", layer->toString());
        layers.emplace(layers.begin() + insertIndex, layer);
        insertIndex++;
        layer->onAttach();
        ST_INFO(ST_TAG, "Pushed layer [{0}]", layer->getName());
    }

    void LayerStack::popLayer(Layer* layer) {
        ST_DEBUG(ST_TAG, "Popping layer [{0}]", layer->toString());
        auto iterator = std::find(layers.begin(), layers.begin() + insertIndex, layer);
        if (iterator != layers.end()) {
            layer->onDetach();
            delete layer;
            layers.erase(iterator);
            insertIndex--;
        }
        ST_INFO(ST_TAG, "Popped layer [{0}]", layer->getName());
    }

}
#include "Log.h"
#include "LayerStack.h"

namespace Darkle {

    LayerStack::LayerStack() : layers{}, insertIndex(0) {
        LOG_TRACE(TAG, "Created");
    }

    LayerStack::~LayerStack() {
        LOG_TRACE(TAG, "Destroying");
        for (Layer* layer : layers) {
            layer->onDetach();
            delete layer;
        }
        LOG_TRACE(TAG, "Destroyed");
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
        LOG_DEBUG(TAG, "Pushing layer [{0}]", layer->toString());
        layers.emplace(layers.begin() + insertIndex, layer);
        insertIndex++;
        layer->onAttach();
        LOG_INFO(TAG, "Pushed layer [{0}]", layer->getName());
    }

    void LayerStack::popLayer(Layer* layer) {
        LOG_DEBUG(TAG, "Popping layer [{0}]", layer->toString());
        auto iterator = std::find(layers.begin(), layers.begin() + insertIndex, layer);
        if (iterator != layers.end()) {
            layer->onDetach();
            delete layer;
            layers.erase(iterator);
            insertIndex--;
        }
        LOG_INFO(TAG, "Popped layer [{0}]", layer->getName());
    }

}
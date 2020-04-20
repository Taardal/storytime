#include "system/Log.h"
#include "LayerStack.h"

namespace storytime
{
    LayerStack::LayerStack()
            : layers{}, insertIndex(0)
    {
    }

    LayerStack::~LayerStack()
    {
        for (Layer* layer : layers)
        {
            layer->OnDetach();
            delete layer;
        }
    }

    std::vector<Layer*> LayerStack::GetLayers() const
    {
        return layers;
    }

    std::vector<Layer*>::const_iterator LayerStack::begin() const
    {
        return layers.begin();
    }

    std::vector<Layer*>::const_iterator LayerStack::end() const
    {
        return layers.end();
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        layers.emplace(layers.begin() + insertIndex, layer);
        insertIndex++;
        layer->OnAttach();
        ST_LOG_INFO(ST_TAG, "Pushed layer [{0}]", layer->GetName());
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        auto iterator = std::find(layers.begin(), layers.begin() + insertIndex, layer);
        if (iterator != layers.end())
        {
            layer->OnDetach();
            delete layer;
            layers.erase(iterator);
            insertIndex--;
        }
        ST_LOG_INFO(ST_TAG, "Popped layer [{0}]", layer->GetName());
    }

}
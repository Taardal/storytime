#include "system/log.h"
#include "LayerStack.h"

namespace Storytime
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

    void LayerStack::PushLayer(Layer* layer)
    {
        layers.emplace(layers.begin() + insertIndex, layer);
        insertIndex++;
        layer->OnAttach();
        ST_LOG_INFO("Pushed layer [{0}]", layer->GetName());
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
        ST_LOG_INFO("Popped layer [{0}]", layer->GetName());
    }

    std::vector<Layer*>::const_iterator LayerStack::begin() const
    {
        return layers.begin();
    }

    std::vector<Layer*>::const_iterator LayerStack::end() const
    {
        return layers.end();
    }

    std::vector<Layer*>::const_reverse_iterator LayerStack::rbegin() const
    {
        return layers.rbegin();
    }

    std::vector<Layer*>::const_reverse_iterator LayerStack::rend() const
    {
        return layers.rend();
    }

}
#include "st_quad_vertex.h"

namespace Storytime {

    // A vertex binding describes at which rate to load data from memory throughout the vertices.
    // It specifies the number of bytes between data entries and whether to move to the next data
    // entry after each vertex or after each instance.
    //
    // VkVertexInputBindingDescription:
    // [binding] The index of the binding in the array of bindings.
    // [stride] The number of bytes from one entry to the next.
    // [inputRate] Move to the next data entry after each vertex.
    //
    VkVertexInputBindingDescription QuadVertex::getBindingDescription() {
        VkVertexInputBindingDescription vertex_input_binding_description{};
        vertex_input_binding_description.binding = 0;
        vertex_input_binding_description.stride = sizeof(QuadVertex);
        vertex_input_binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return vertex_input_binding_description;
    }

    // An attribute description describes how to extract a vertex attribute from a chunk of vertex
    // data originating from a binding description.
    //
    // VkVertexInputAttributeDescription:
    // [binding] From which binding the per-vertex data comes (i.e. VkVertexInputBindingDescription).
    // [location] The location directive of the input in the vertex shader (i.e. `layout(location = 0)`).
    // [format] The type of data for the attribute (specified using the same enumeration as color formats).
    // [offset] The number of bytes since the start of the per-vertex data to read from.
    //
    std::vector<VkVertexInputAttributeDescription> QuadVertex::getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> vertex_input_attribute_descriptions(2);

        vertex_input_attribute_descriptions[0].binding = 0;
        vertex_input_attribute_descriptions[0].location = 0;
        vertex_input_attribute_descriptions[0].format = get_vk_format("vec2");
        vertex_input_attribute_descriptions[0].offset = offsetof(QuadVertex, position);

        vertex_input_attribute_descriptions[1].binding = 0;
        vertex_input_attribute_descriptions[1].location = 1;
        vertex_input_attribute_descriptions[1].format = get_vk_format("vec3");
        vertex_input_attribute_descriptions[1].offset = offsetof(QuadVertex, color);

        return vertex_input_attribute_descriptions;
    }
}

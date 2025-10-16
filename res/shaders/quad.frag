#version 450

layout(set = 0, binding = 1) uniform sampler2D texture_samplers[16];

layout(location = 0) in vec4 color;
layout(location = 1) in vec2 texture_coordinate;
layout(location = 2) in float texture_index;

layout(location = 0) out vec4 fragment_color;

void main() {
    fragment_color = texture(texture_samplers[int(texture_index)], texture_coordinate) * color;
}
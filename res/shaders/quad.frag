#version 450

// Set 1: Per-batch.
layout(set = 1, binding = 0) uniform sampler2D texture_samplers[16];

// Data exposed from the vertex shader.
layout(location = 0) in struct VertexOutput {
    vec4 color;
    vec2 texture_coordinate;
    float texture_index;
} vertex_output;

// The final fragment color.
layout(location = 0) out vec4 fragment_color;

void main() {
    fragment_color = texture(texture_samplers[int(vertex_output.texture_index)], vertex_output.texture_coordinate) * vertex_output.color;
}
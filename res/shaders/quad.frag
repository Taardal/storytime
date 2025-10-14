#version 450

layout(set = 0, binding = 1) uniform sampler2D texture_sampler;

layout(location = 0) in vec4 fragment_color;
layout(location = 1) in vec2 fragment_texture_coordinate;

layout(location = 0) out vec4 color;

void main() {
    color = fragment_color;
}
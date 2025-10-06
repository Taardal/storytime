#version 450

layout(set = 0, binding = 1) uniform sampler2D texture_sampler;

layout(location = 0) in vec3 fragment_color;
layout(location = 1) in vec2 fragment_texture_coordinate;

layout(location = 0) out vec4 color;

void main() {
    color = texture(texture_sampler, fragment_texture_coordinate * 2);
}
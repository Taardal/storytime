#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject{
    mat4 model;
    mat4 view;
    mat4 projection;
} ubo;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texture_coordinate;

layout(location = 0) out vec3 fragment_color;
layout(location = 1) out vec2 fragment_texture_coordinate;

void main() {
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(position, 1.0);
    fragment_color = color;
    fragment_texture_coordinate = texture_coordinate;
}
#version 450

layout(set = 0, binding = 0) uniform ViewProjection{
    mat4 view;
    mat4 projection;
} view_projection;

// Binding 0: static vertex buffer
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture_coordinate;

// Binding 1: Instance vertex buffer
layout(location = 2) in vec4 model_1;
layout(location = 3) in vec4 model_2;
layout(location = 4) in vec4 model_3;
layout(location = 5) in vec4 model_4;
layout(location = 6) in vec4 color;
layout(location = 7) in vec4 texture_rectangle;
layout(location = 8) in float texture_index;

layout(location = 0) out vec4 fragment_color;
layout(location = 1) out vec2 fragment_texture_coordinate;
layout(location = 2) out float fragment_texture_index;

void main() {
    mat4 model = mat4(model_1, model_2, model_3, model_4);
    gl_Position = view_projection.projection * view_projection.view * model * position;

    fragment_color = color;
    fragment_texture_coordinate = mix(texture_rectangle.xy, texture_rectangle.zw, texture_coordinate);
    fragment_texture_index = texture_index;
}
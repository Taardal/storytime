#version 450

layout(push_constant) uniform ViewProjection{
    mat4 view;
    mat4 projection;
} view_projection;

// Binding 0: Base vertex buffer.
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture_coordinate;

// Binding 1: Instance vertex buffer.
layout(location = 2) in vec4 model_1;
layout(location = 3) in vec4 model_2;
layout(location = 4) in vec4 model_3;
layout(location = 5) in vec4 model_4;
layout(location = 6) in vec4 color;
layout(location = 7) in vec4 texture_rectangle;
layout(location = 8) in float texture_index;

// Data exposed to fragment shader.
layout(location = 0) out struct VertexOutput {
    vec4 color;
    vec2 texture_coordinate;
    float texture_index;
} vertex_output;

// Determine fragment shader texture coordinate (UV).
//
// Definitions:
// - The base vertex texture coordinate represents the normalized coordinate (UV) for a corner of the base quad.
// - The instance vertex texture rectangle represents the normalized coordinates (UVs) of the rectangular area that
//   should be sampled from in the texture (f.ex. a single sprite in a spritesheet).
//
// Problem:
// - We need to determine which corner of the texture rectangle to pass to the fragment shader as an output.
//
// Solution:
// - We use the mix() function to interpolate between the texture rectangle corners (UVs) according to the base vertex corner (UV).
//
// (0,0) -> mix(texture_rectangle.xy, texture_rectangle.zw, vec2(0,0)) -> top-left = vec2(texture_rectangle.x, texture_rectangle.y)
// (1,0) -> mix(texture_rectangle.xy, texture_rectangle.zw, vec2(1,0)) -> top-right = vec2(texture_rectangle.z, texture_rectangle.y)
// (1,1) -> mix(texture_rectangle.xy, texture_rectangle.zw, vec2(1,1)) -> bottom-right = vec2(texture_rectangle.z, texture_rectangle.w)
// (0,1) -> mix(texture_rectangle.xy, texture_rectangle.zw, vec2(0,1)) -> bottom-left = vec2(texture_rectangle.x, texture_rectangle.w)
//
// Example: texture_rectangle = 0.25x, 0.25y, 0.3125z, 0.3125w
//
// (0,0) -> mix(vec2(0.25x, 0.25y), vec2(0.3125z, 0.3125w), vec2(0,0)) -> top-left = vec2(0.25, 0.25)
// (1,0) -> mix(vec2(0.25x, 0.25y), vec2(0.3125z, 0.3125w), vec2(1,0)) -> top-right = vec2(0.3125, 0.25)
// (1,1) -> mix(vec2(0.25x, 0.25y), vec2(0.3125z, 0.3125w), vec2(1,1)) -> bottom-right = vec2(0.3125, 0.3125)
// (0,1) -> mix(vec2(0.25x, 0.25y), vec2(0.3125z, 0.3125w), vec2(0,1)) -> bottom-left = vec2(0.25, 0.3125)
//
vec2 get_texture_coordinate() {
    return mix(texture_rectangle.xy, texture_rectangle.zw, texture_coordinate);
}

void main() {
    mat4 model = mat4(model_1, model_2, model_3, model_4);
    gl_Position = view_projection.projection * view_projection.view * model * position;

    vertex_output.color = color;
    vertex_output.texture_index = texture_index;
    vertex_output.texture_coordinate = get_texture_coordinate();
}

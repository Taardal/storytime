#version 410

struct Vertex {
    vec4 color;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform mat4 viewProjection;

out Vertex vertex = Vertex(color);

void main() {
    gl_Position = viewProjection * vec4(position, 1.0);
}
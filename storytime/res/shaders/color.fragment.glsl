#version 410

struct Vertex {
    vec4 color;
};

layout(location = 0) out vec4 color;

in Vertex vertex;

void main() {
    color = vertex.color;
}
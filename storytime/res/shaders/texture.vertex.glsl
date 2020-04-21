#version 410

struct Vertex {
    vec4 color;
    vec2 textureCoordinate;
    float textureIndex;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 textureCoordinate;
layout(location = 3) in float textureIndex;

out Vertex vertex = Vertex(color, textureCoordinate, textureIndex);

uniform mat4 viewProjection;

void main() {
    gl_Position = viewProjection * vec4(position, 1.0);
}
#version 330 core

struct Vertex {
    vec4 color;
    vec2 textureCoordinate;
    float textureIndex;
};

layout(location = 0) out vec4 color;

in Vertex vertex;

uniform sampler2D textureSamplers[16];

void main() {
    color = texture(textureSamplers[int(vertex.textureIndex)], vertex.textureCoordinate) * vertex.color;
}
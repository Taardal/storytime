#version 410

struct Vertex {
    vec4 Color;
    vec2 TextureCoordinate;
    float TextureIndex;
    float TilingFactor;
};

layout(location = 0) out vec4 color;

in Vertex vertex;

uniform sampler2D textureSamplers[16];

void main() {
    color = texture(textureSamplers[int(vertex.TextureIndex)], vec2(vertex.TextureCoordinate.x, 1.0 - vertex.TextureCoordinate.y) * vertex.TilingFactor) * vertex.Color;
}
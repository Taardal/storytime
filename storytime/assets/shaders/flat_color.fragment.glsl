#version 330 core

layout(location = 0) out vec4 colorOutput;

uniform vec4 color;

void main() {
    colorOutput = color;
}
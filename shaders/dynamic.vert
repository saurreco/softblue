#version 330 core

layout (location = 0) in vec3 cubeCoords;
layout (location = 1) in vec2 cubeNormal;

out vec3 textureCoords;
out vec2 textureNormal;

uniform mat4 viewProj;

void main(void)
{
    textureCoords = cubeCoords;
    textureNormal = cubeNormal;
    gl_Position = viewProj * vec4(cubeCoords, 1.f);
}

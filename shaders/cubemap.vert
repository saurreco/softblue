#version 330 core

layout (location = 0) in vec3 cubeCoords;

out vec3 textureCoords;

uniform mat4 viewProj;

void main(void)
{
    textureCoords = cubeCoords;
    gl_Position = viewProj * vec4(cubeCoords, 1.f);
}

#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

out vec3 wsPos;
out vec3 wsNormal;

uniform mat4 viewProj;

void main() {
    wsPos = pos;
    wsNormal = normal;
    gl_Position = viewProj * vec4(pos, 1);
}

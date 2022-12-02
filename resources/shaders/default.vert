#version 330 core

layout(location = 0) in vec3 osPos;
layout(location = 1) in vec3 osNormal;

uniform mat4 mvp;

out vec3 wsPos;

void main() {
    gl_Position = mvp * vec4(osPos, 1);
}

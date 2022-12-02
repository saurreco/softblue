#version 330 core

layout(location = 0) in vec3 osPos;
layout(location = 1) in vec3 osNormal;

out vec3 wsPos;
out vec3 wsNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 normalTransformMatrix;

void main() {
    wsNormal = vec3(normalTransformMatrix * vec4(normalize(vec4(osPos, 1))));
    wsPos = vec3(modelMatrix * vec4(osPos, 1));

    wsPos = osPos;
    wsNormal = normalize(osPos);
    gl_Position = projMatrix * viewMatrix * vec4(osPos, 1);
}

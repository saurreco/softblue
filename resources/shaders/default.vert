#version 330 core

layout(location = 0) in vec3 osPos;
layout(location = 1) in vec3 osNormal;
layout(location = 2) in vec2 texCoords;

out vec3 wsPos;
out vec3 wsNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 normalTransform;

void main() {

    wsPos = vec3(model * vec4(osPos,1));
    wsNormal = vec3(normalTransform * vec4(osNormal, 1));
    gl_Position = proj * view * model * vec4(osPos, 1);

}

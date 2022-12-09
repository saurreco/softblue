#version 330 core

in vec3 textureCoords;
in vec2 textureNormal;
out vec4 fragColor;

uniform sampler2D cubemap_texture_side;

void main(void)
{
    fragColor = texture(cubemap_texture_side, textureNormal.xy);
    float u = floor(textureNormal[0] * 10) / 10.f;
    float v = floor(textureNormal[1] * 10) / 10.f;
}

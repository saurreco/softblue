#version 330 core

in vec3 textureCoords;
out vec4 fragColor;

uniform samplerCube cubemap_texture;

void main(void)
{
    fragColor = texture(cubemap_texture, textureCoords);
}

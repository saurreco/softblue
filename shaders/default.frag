#version 330 core


in vec3 wsPos;
in vec3 wsNormal;
out vec4 fragColor;

uniform vec3 lightDir;
uniform vec4 lightColor;
uniform vec3 camPos;

uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;

float clamp(float v) {
    return max(0, min(1, v));
}

void main() {

    float ka = 0.5;
    float kd = 0.5;
    float ks = 0.5;
    float n = 100; /* shininess */

    fragColor = vec4(0);

    vec4 ambient = ka * ambient;
    float diffuseDot = clamp(dot(wsNormal, -normalize(lightDir)));
    vec4 diffuse = lightColor * kd * diffuse * diffuseDot;

    vec3 r = reflect(normalize(lightDir), normalize(wsNormal));
    float specularDot = max(0, min(1, dot(r, normalize(camPos - wsPos))));
    vec4 specular = lightColor * ks * specular *  clamp(pow(specularDot, n));

    fragColor += specular;
    fragColor += ambient;
    fragColor += diffuse;
}

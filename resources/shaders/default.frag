#version 330 core

in vec3 wsPos;
in vec3 wsNormal;

out vec4 fragColor;

struct Light {
    int type;
    vec4 color;
    vec3 attenuation;
    vec3 pos;
    vec3 dir;
    float penumbra;
    float angle;
};

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform Light lights[8];
uniform Material material;
uniform vec3 camPos;
uniform float numLights;
uniform float ka;
uniform float kd;
uniform float ks;

float clamp(float v) {
    return min(1, max(0, v));
}

float falloff(float x, float inner, float outer) {
    return -2 * pow((x - inner) / (outer - inner), 3) +
            3 * pow((x - inner) / (outer - inner), 2);
}

void main() {

    fragColor = vec4(0.0);
    vec3 normal = normalize(wsNormal);
    vec4 ambient = vec4(0.0);
    vec4 diffuse = vec4(0.0);
    vec4 specular = vec4(0.0);
    vec3 lightDir;
    float fatt = 1;
    float intensity = 1;

    ambient += material.ambient * ka;

    for (int i = 0; i < numLights; i++) {
        Light light = lights[i];
        vec4 lightColor = light.color;

        if (light.type == 0) {
            lightDir = normalize(light.dir);
        }

        if (light.type == 1 || light.type == 2) {

            lightDir = normalize(wsPos - vec3(light.pos));
            /* attenuation */
            float d = length(vec3(light.pos) - wsPos);
            fatt = clamp(1 / (light.attenuation.z * d * d +
                              light.attenuation.y * d +
                              light.attenuation.x));
        }

        if (light.type == 2) {
            lightDir = normalize(light.dir);
            vec3 tmp = normalize(wsPos - vec3(light.pos));
            float x = acos(dot(lightDir, tmp));
            float inner = light.angle - light.penumbra;
            float outer = light.angle;
            if (x <= inner) {
                intensity = 1;
            } else if (x <= outer) {
                intensity = (1 - falloff(x, inner, outer));
            } else {
                intensity = 0;
            }
        }

        lightColor = lightColor * intensity;

        /* diffuse */
        diffuse += fatt * kd * material.diffuse *
                   lightColor * clamp(dot(-lightDir, normal));

        /* specular */
        vec3 r = reflect(lightDir, normal);
        vec3 camDir = normalize(vec3(camPos) - wsPos);
        float shininess = material.shininess == 0 ? 1 : material.shininess;
        specular += fatt * ks * lightColor * material.specular *
                    pow(clamp(dot(r, camDir)), shininess);
    }

    fragColor += ambient;
    fragColor += diffuse;
    fragColor += specular;
}

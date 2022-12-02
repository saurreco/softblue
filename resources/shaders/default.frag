#version 330 core


in vec3 wsPos;
in vec3 wsNormal;
out vec4 fragColor;

uniform float m_ka;
uniform float m_kd;
uniform float m_ks;

uniform vec3 lightDir;
uniform vec4 lightColor;
uniform vec3 camPos;
uniform float shininess;

uniform vec4 objectAmbient;
uniform vec4 objectDiffuse;
uniform vec4 objectSpecular;

void main() {

    fragColor = vec4(0.0);

//    vec4 ambient = vec4(m_ka, m_ka, m_ka, 1);
//    vec3 lightDir = vec3(normalize(lightPos - camPos));
//    float d = m_kd * max(0, min(1, dot(normalize(wsNormal), lightDir)));
//    vec4 diffuse = vec4(d, d, d, 1);

//    vec3 r = reflect(-lightDir, normalize(wsNormal));
//    float daa = max(0, min(1, dot(r, normalize(vec3(camPos) - wsPos))));
//    float s = m_ks * pow(daa, shininess);
//    vec4 specular = vec4(s, s, s, 1);
    vec4 ambient = m_ka * objectAmbient;

//    vec3 lightDir = vec3(normalize(lightPos - wsPos));
    float diffuseDot = max(0, min(1, dot(wsNormal, -normalize(lightDir))));
    vec4 diffuse = lightColor * m_kd * objectDiffuse * diffuseDot;

    vec3 r = reflect(normalize(lightDir), normalize(wsNormal));
    float specularDot = max(0, min(1, dot(r, normalize(camPos - wsPos))));
    float powered;
    if (shininess <= 0) {
        powered = 1; // is this correct?
    } else {
        powered = pow(specularDot, shininess);
    }
    vec4 specular = lightColor * m_ks * objectSpecular * powered;
    fragColor = specular;
    fragColor += ambient;
    fragColor += diffuse;

    fragColor[0] = clamp(fragColor[0], 0, 1);
    fragColor[1] = clamp(fragColor[1], 0, 1);
    fragColor[2] = clamp(fragColor[2], 0, 1);
    fragColor[3] = clamp(fragColor[3], 0, 1);
}

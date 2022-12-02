#version 330 core


in vec3 wsPos;
in vec3 wsNormal;
out vec4 fragColor;

uniform float m_ka;
uniform float m_kd;
uniform float m_ks;

uniform vec4 lightPos;
uniform vec4 camPos;
uniform float shininess;

void main() {

    fragColor = vec4(0.0);

    vec4 ambient = vec4(m_ka, m_ka, m_ka, 1);
    vec3 lightDir = vec3(normalize(lightPos - camPos));
    float d = m_kd * max(0, min(1, dot(normalize(wsNormal), lightDir)));
    vec4 diffuse = vec4(d, d, d, 1);

    vec3 r = reflect(-lightDir, normalize(wsNormal));
    float daa = max(0, min(1, dot(r, normalize(vec3(camPos) - wsPos))));
    float s = m_ks * pow(daa, shininess);
    vec4 specular = vec4(s, s, s, 1);

    fragColor += ambient;
    fragColor += diffuse;
    fragColor += specular;
}

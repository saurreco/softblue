#include "shader.h"
#include <glm/gtx/string_cast.hpp>


void Shader::set(std::string vert, std::string frag) {
    shader = ShaderLoader::createShaderProgram(vert.c_str(), frag.c_str());
}


void Shader::bind() {
    glUseProgram(shader);
}

void Shader::unbind() {
    glUseProgram(0);
}

void Shader::addUniformFloat(std::string uniform, float v) {
    glUniform1f(glGetUniformLocation(shader, uniform.c_str()), v);
}

void Shader::addUniformInt(std::string uniform, int v) {
    glUniform1i(glGetUniformLocation(shader, uniform.c_str()), v);
}

void Shader::addUniformVec3(std::string uniform, glm::vec3& v) {
    glUniform3fv(glGetUniformLocation(shader, uniform.c_str()), 1, &v[0]);
}

void Shader::addUniformVec4(std::string uniform, glm::vec4& v) {
    glUniform4fv(glGetUniformLocation(shader, uniform.c_str()), 1, &v[0]);
}

void Shader::addUniformMat4(std::string uniform, glm::mat4& v) {
    glUniformMatrix4fv(glGetUniformLocation(shader, uniform.c_str()), 1, GL_FALSE, &v[0][0]);
}

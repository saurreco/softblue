#pragma once

#include "mesh.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "utils/shaderloader.h"

class Shader {
public:
    void set(std::string vert, std::string frag);
    void bind();
    void unbind();
    void addUniformFloat(std::string uniform, float v);
    void addUniformInt(std::string uniform, int v);
    void addUniformVec3(std::string uniform, glm::vec3& v);
    void addUniformVec4(std::string uniform, glm::vec4& v);
    void addUniformMat4(std::string uniform, glm::mat4& v);
private:
    GLuint shader;
};

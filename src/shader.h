#pragma once

#include "mesh.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "utils/shaderloader.h"
#include "utils/errorthrowing.h"

enum ShaderType {
    MAIN_SHADER,
    CUBEMAP_SHADER,
};

class Shader {
public:
    void set(ShaderType shaderType, std::string vert, std::string frag);
    void bind(ShaderType shaderType);
    void unbind();
    void addUniformFloat(ShaderType shaderType, std::string uniform, float v);
    void addUniformInt(ShaderType shaderType, std::string uniform, int v);
    void addUniformVec3(ShaderType shaderType, std::string uniform, glm::vec3& v);
    void addUniformVec4(ShaderType shaderType, std::string uniform, glm::vec4& v);
    void addUniformMat4(ShaderType shaderType, std::string uniform, glm::mat4& v);
private:
    GLuint mainShader;
    GLuint cubemapShader;
};

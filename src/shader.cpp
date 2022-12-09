#include "shader.h"
#include <glm/gtx/string_cast.hpp>


void Shader::set(ShaderType shaderType, std::string vert, std::string frag) {
    if (shaderType == ShaderType::MAIN_SHADER) {
        mainShader = ShaderLoader::createShaderProgram(vert.c_str(), frag.c_str());
    } else if (shaderType == ShaderType::CUBEMAP_SHADER) {
        cubemapShader = ShaderLoader::createShaderProgram(vert.c_str(), frag.c_str());
    } else if (shaderType == ShaderType::DYNAMIC_SHADER) {
        dynamicShader = ShaderLoader::createShaderProgram(vert.c_str(), frag.c_str());
    }
}


void Shader::bind(ShaderType shaderType) {
    if (shaderType == ShaderType::MAIN_SHADER) {
        glUseProgram(mainShader);
    } else if (shaderType == ShaderType::CUBEMAP_SHADER) {
        glUseProgram(cubemapShader);
    } else if (shaderType == ShaderType::DYNAMIC_SHADER) {
        glUseProgram(dynamicShader);
    }
}

void Shader::unbind() {
    glUseProgram(0);
}

void Shader::addUniformFloat(ShaderType shaderType, std::string uniform, float v) {
    if (shaderType == ShaderType::MAIN_SHADER) {
        GLint loc = glGetUniformLocation(mainShader, uniform.c_str());
        ErrorThrowing::uniformNotFound(loc, uniform);
        glUniform1f(loc, v);
    } else if (shaderType == ShaderType::CUBEMAP_SHADER) {
        GLint loc = glGetUniformLocation(cubemapShader, uniform.c_str());
        ErrorThrowing::uniformNotFound(loc, uniform);
        glUniform1f(loc, v);
    } else if (shaderType == ShaderType::DYNAMIC_SHADER) {
        GLint loc = glGetUniformLocation(dynamicShader, uniform.c_str());
        ErrorThrowing::uniformNotFound(loc, uniform);
        glUniform1f(loc, v);
    }
}

void Shader::addUniformInt(ShaderType shaderType, std::string uniform, int v) {
    if (shaderType == ShaderType::MAIN_SHADER) {
        GLint loc = glGetUniformLocation(mainShader, uniform.c_str());
        ErrorThrowing::uniformNotFound(loc, uniform);
        glUniform1i(loc, v);
    } else if (shaderType == ShaderType::CUBEMAP_SHADER) {
        GLint loc = glGetUniformLocation(cubemapShader, uniform.c_str());
        ErrorThrowing::uniformNotFound(loc, uniform);
        glUniform1i(loc, v);
    } else if (shaderType == ShaderType::DYNAMIC_SHADER) {
        GLint loc = glGetUniformLocation(dynamicShader, uniform.c_str());
        ErrorThrowing::uniformNotFound(loc, uniform);
        glUniform1i(loc, v);
    }
}

void Shader::addUniformVec3(ShaderType shaderType, std::string uniform, glm::vec3& v) {
    if (shaderType == ShaderType::MAIN_SHADER) {
        GLint loc = glGetUniformLocation(mainShader, uniform.c_str());
        ErrorThrowing::uniformNotFound(loc, uniform);
        glUniform3fv(loc, 1, &v[0]);
    } else if (shaderType == ShaderType::CUBEMAP_SHADER) {
        GLint loc = glGetUniformLocation(cubemapShader, uniform.c_str());
        ErrorThrowing::uniformNotFound(loc, uniform);
        glUniform3fv(loc, 1, &v[0]);
    } else if (shaderType == ShaderType::DYNAMIC_SHADER) {
        GLint loc = glGetUniformLocation(dynamicShader, uniform.c_str());
        ErrorThrowing::uniformNotFound(loc, uniform);
        glUniform3fv(loc, 1, &v[0]);
    }
}

void Shader::addUniformVec4(ShaderType shaderType, std::string uniform, glm::vec4& v) {
    if (shaderType == ShaderType::MAIN_SHADER) {
        GLint loc = glGetUniformLocation(mainShader, uniform.c_str());
        ErrorThrowing::uniformNotFound(loc, uniform);
        glUniform4fv(loc, 1, &v[0]);
    } else if (shaderType == ShaderType::CUBEMAP_SHADER) {
        GLint loc = glGetUniformLocation(cubemapShader, uniform.c_str());
        ErrorThrowing::uniformNotFound(loc, uniform);
        glUniform4fv(loc, 1, &v[0]);
    } else if (shaderType == ShaderType::DYNAMIC_SHADER) {
        GLint loc = glGetUniformLocation(dynamicShader, uniform.c_str());
        ErrorThrowing::uniformNotFound(loc, uniform);
        glUniform4fv(loc, 1, &v[0]);
    }
}

void Shader::addUniformMat4(ShaderType shaderType, std::string uniform, glm::mat4& v) {
    if (shaderType == ShaderType::MAIN_SHADER) {
        GLint loc = glGetUniformLocation(mainShader, uniform.c_str());
        ErrorThrowing::uniformNotFound(loc, uniform);
        glUniformMatrix4fv(loc, 1, GL_FALSE, &v[0][0]);
    } else if (shaderType == ShaderType::CUBEMAP_SHADER) {
        GLint loc = glGetUniformLocation(cubemapShader, uniform.c_str());
        ErrorThrowing::uniformNotFound(loc, uniform);
        glUniformMatrix4fv(loc, 1, GL_FALSE, &v[0][0]);
    } else if (shaderType == ShaderType::DYNAMIC_SHADER) {
        GLint loc = glGetUniformLocation(dynamicShader, uniform.c_str());
        ErrorThrowing::uniformNotFound(loc, uniform);
        glUniformMatrix4fv(loc, 1, GL_FALSE, &v[0][0]);
    }
}

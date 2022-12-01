#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "utils/shaderloader.h"

class FBO {
public:
    FBO(int w, int h) {
        this->fbo_width = w;
        this->fbo_height = h;
        defaultFBO = 2;
    }
    void makeFBO();
    void deleteFBO();
    void paintTexture(int f);
    void initScreenGeometry();
    GLuint fbo_width;
    GLuint fbo_height;
    GLuint fbo;
    GLuint fbo_texture;
    GLuint fbo_renderbuffer;

    GLuint screen_vbo;
    GLuint screen_vao;
    GLuint texture_shader;

    GLuint defaultFBO;
};

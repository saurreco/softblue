#include "dynamiccubemap.h"
#include <iostream>
#include "debug.h"

void DynamicCubemap::genFbos() {
    // depth buffer
    glGenRenderbuffers(1, &fbo_rb_cube);
    glBindRenderbuffer(GL_RENDERBUFFER, fbo_rb_cube);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->textureSideLength, this->textureSideLength);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    Debug::glErrorCheck();

    // texture
    // create the cubemap
    glGenTextures(1, &this->fbo_tex_cube);
    glActiveTexture(GL_TEXTURE1); // texture slot 1
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->fbo_tex_cube);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    for (int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, this->textureSideLength, this->textureSideLength, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    glGenFramebuffers(1, &this->fbo_cube);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo_cube);
    // attach buffer & tex to fbo
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->fbo_rb_cube);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->fbo_tex_cube, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFbo);
}

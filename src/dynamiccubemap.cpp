#include "dynamiccubemap.h"
#include <iostream>

void DynamicCubemap::genFbos() {
    // FOR NOW: generate only top fbo

    // depth buffer
    glGenRenderbuffers(1, &fbo_rb_top);
    glBindRenderbuffer(GL_RENDERBUFFER, fbo_rb_top);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->textureSideLength, this->textureSideLength);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // texture
    // create the cubemap
    glGenTextures(1, &this->fbo_tex_top);
    glActiveTexture(GL_TEXTURE1); // texture slot 1
    glBindTexture(GL_TEXTURE_2D, this->fbo_tex_top);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->textureSideLength, this->textureSideLength, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &this->fbo_top);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo_top);
    // attach buffer & tex to fbo
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->fbo_rb_top);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->fbo_tex_top, 0);

    GLenum fboState = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboState != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "incomplete" << std::endl;
        switch (fboState) {
        case GL_FRAMEBUFFER_UNDEFINED:
            std::cout << "undefined" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cout << "incomplete attachment" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cout << "incomplete missing attachment" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            std::cout << "incomplete draw buffer" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            std::cout << "incomplete read buffer" << std::endl;
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cout << "unsupported" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            std::cout << "incomplete multisample" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            std::cout << "incomplete layer targets" << std::endl;
            break;
        default:
            std::cout << "not sure" << std::endl;
            break;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFbo);
}

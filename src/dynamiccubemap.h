#pragma once
#ifndef DYNAMICCUBEMAP_H
#define DYNAMICCUBEMAP_H

#include <GL/glew.h>

class DynamicCubemap {
public:
    GLuint fbo_top;
    GLuint fbo_rb_top;
    GLuint fbo_tex_top;

    GLuint fbo_bottom;
    GLuint fbo_front;
    GLuint fbo_back;
    GLuint fbo_left;
    GLuint fbo_right;

    void genFbos();

    int textureSideLength;
    GLuint defaultFbo = 2; // FOR NOW IT'S 2
    GLuint cubemap_texture;

    GLuint dynamic_cubemap_texture;

//    void loadUniforms();
};

#endif // DYNAMICCUBEMAP_H

#pragma once
#ifndef DYNAMICCUBEMAP_H
#define DYNAMICCUBEMAP_H

#include <GL/glew.h>

class DynamicCubemap {
public:
    GLuint fbo_cube;
    GLuint fbo_rb_cube;
    GLuint fbo_tex_cube;

    void genFbos();

    int textureSideLength;
    GLuint defaultFbo = 2; // FOR NOW IT'S 2

    GLuint dynamic_cubemap_texture;
};

#endif // DYNAMICCUBEMAP_H

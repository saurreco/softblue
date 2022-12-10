#pragma once
#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <GL/glew.h>
#include <QImage>
#include <iostream>
#include "shader.h"
#include "dynamiccubemap.h"

class Cubemap {
public:
    void fillCubeMap(const char* front,
                       const char* back,
                       const char* top,
                       const char* bottom,
                       const char* left,
                       const char* right);
    bool loadCubeMapSide(GLuint texture, GLenum side_target, const char* file_name);

    void initCubeMap();
    void drawCubeMap();

    GLuint cubemap_texture;
    GLuint cubemapVbo;
    GLuint cubemapVao;
    GLuint cubemapTop_vbo;
    GLuint cubemapTop_vao;

    void bindDynamic();
    void unbindDynamic();
//    void bindVaoDynamic();
//    void unbindVaoDynamic();

    void bindCubesideTex(); // currently: TOP

    void initCubeSide(); // currently: TOP

    int cubemapSideLength; // it's a cube => width = height

    DynamicCubemap* dynamicCubemap = new DynamicCubemap();
private:
    void initDynamicCubemap(); // called in fillCubeMap();
};

#endif // CUBEMAP_H

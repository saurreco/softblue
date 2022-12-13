#include "cubemap.h"
#include <ostream>
#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"
#include "debug.h"

void Cubemap::fillCubeMap(
    const char* front,
    const char* back,
    const char* top,
    const char* bottom,
    const char* left,
    const char* right) {
    // generate a cube-map texture to hold all the sides
    glGenTextures(1, &this->cubemap_texture);

    // binding
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemap_texture);

    // load each image and copy into a side of the cube-map texture
    loadCubeMapSide(this->cubemap_texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
    loadCubeMapSide(this->cubemap_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
    loadCubeMapSide(this->cubemap_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
    loadCubeMapSide(this->cubemap_texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
    loadCubeMapSide(this->cubemap_texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
    loadCubeMapSide(this->cubemap_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
    // format cube map texture
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // unbinding
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    // init dynamic cubemap
    this->initDynamicCubemap();
}

bool Cubemap::loadCubeMapSide(GLuint texture, GLenum side_target, const char* file_name) {
    int x, y, n;
    int force_channels = 4;
    unsigned char*  image_data = stbi_load(
        file_name, &x, &y, &n, force_channels);
    if (!image_data) {
        std::cerr << "ERROR: could not load " << file_name << std::endl;
        return false;
    }
    // non-power-of-2 dimensions check
    if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
        std::cerr << "WARNING: image " << file_name << " is not power-of-2 dimensions " << std::endl;
    }

    this->cubemapSideLength = x;

    // copy image data into 'target' side of cube map
    glTexImage2D(
        side_target,
        0,
        GL_RGBA,
        x,
        y,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        image_data);
    free(image_data);
    return true;
}

void Cubemap::initCubeMap() {
    float points[] = {
      -10.0f,  10.0f, -10.0f,
      -10.0f, -10.0f, -10.0f,
       10.0f, -10.0f, -10.0f,
       10.0f, -10.0f, -10.0f,
       10.0f,  10.0f, -10.0f,
      -10.0f,  10.0f, -10.0f,

      -10.0f, -10.0f,  10.0f,
      -10.0f, -10.0f, -10.0f,
      -10.0f,  10.0f, -10.0f,
      -10.0f,  10.0f, -10.0f,
      -10.0f,  10.0f,  10.0f,
      -10.0f, -10.0f,  10.0f,

       10.0f, -10.0f, -10.0f,
       10.0f, -10.0f,  10.0f,
       10.0f,  10.0f,  10.0f,
       10.0f,  10.0f,  10.0f,
       10.0f,  10.0f, -10.0f,
       10.0f, -10.0f, -10.0f,

      -10.0f, -10.0f,  10.0f,
      -10.0f,  10.0f,  10.0f,
       10.0f,  10.0f,  10.0f,
       10.0f,  10.0f,  10.0f,
       10.0f, -10.0f,  10.0f,
      -10.0f, -10.0f,  10.0f,

      -10.0f,  10.0f, -10.0f,
       10.0f,  10.0f, -10.0f,
       10.0f,  10.0f,  10.0f,
       10.0f,  10.0f,  10.0f,
      -10.0f,  10.0f,  10.0f,
      -10.0f,  10.0f, -10.0f,

      -10.0f, -10.0f, -10.0f,
      -10.0f, -10.0f,  10.0f,
       10.0f, -10.0f, -10.0f,
       10.0f, -10.0f, -10.0f,
      -10.0f, -10.0f,  10.0f,
       10.0f, -10.0f,  10.0f
    };
    glGenBuffers(1, &cubemapVbo);
    glGenVertexArrays(1, &cubemapVao);
    glBindBuffer(GL_ARRAY_BUFFER, cubemapVbo);
    glBindVertexArray(cubemapVao);

    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), reinterpret_cast<void *>(0));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Cubemap::drawCubeMap() {
    glBindVertexArray(cubemapVao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindVertexArray(0);
}

void Cubemap::initDynamicCubemap() {
    this->dynamicCubemap->textureSideLength = cubemapSideLength;
    this->dynamicCubemap->genFbos();
}

void Cubemap::bindDynamic() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->dynamicCubemap->fbo_cube);
}

void Cubemap::unbindDynamic() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->dynamicCubemap->defaultFbo);
}

void Cubemap::bindCubesideTex() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->dynamicCubemap->fbo_tex_cube);
}

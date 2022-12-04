#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Mesh
{
public:
    glm::vec3 getVertex(int i);
    glm::vec3 getNormal(int i);
    void setVertex(int i, glm::vec3 pos);
    void setNormal(int i, glm::vec3 normal);

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    int numVertices = 0;
    int numIndices = 0;

protected:
    virtual int pushVertex(glm::vec3 v, glm::vec3 vn);
    virtual void pushTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) = 0;

    void buildGeometry(glm::mat4 model);
    void genBuffers();
    void initBuffers();
    void deleteBuffers();

    std::vector<float> vertices;
    std::vector<int> indices;
};

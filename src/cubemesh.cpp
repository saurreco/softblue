#include "cubemesh.h"

void CubeMesh::pushTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
    glm::vec3 e01 = v0 - v1;
    glm::vec3 e02 = v0 - v2;
    glm::vec3 normal = glm::cross(e01, e02);

    int i0 = this->pushVertex(v0, normal); // this normalization is unnecessary
    int i1 = this->pushVertex(v1, normal);
    int i2 = this->pushVertex(v2, normal);

    this->indices.push_back(i0);
    this->indices.push_back(i1);
    this->indices.push_back(i2);
    numIndices += 3;
}

void CubeMesh::pushFace(glm::vec3 upLeft, glm::vec3 upRight, glm::vec3 downLeft, glm::vec3 downRight) {
    pushTriangle(upLeft, downLeft, upRight);
    pushTriangle(upRight, downLeft, downRight);
}

void CubeMesh::createCube() {
    pushFace(glm::vec3(-0.5f,  0.5f, 0.5f),
             glm::vec3( 0.5f,  0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, 0.5f),
             glm::vec3( 0.5f, -0.5f, 0.5f));

    pushFace(glm::vec3( 0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3( 0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f));

    pushFace(glm::vec3( 0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3( 0.5f, -0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, 0.5f));

    pushFace(glm::vec3(-0.5f, 0.5f, -0.5f),
             glm::vec3( 0.5f, 0.5f, -0.5f),
             glm::vec3(-0.5f, 0.5f, 0.5f),
             glm::vec3( 0.5f, 0.5f, 0.5f));

    pushFace(glm::vec3(0.5f,  0.5f, 0.5f),
             glm::vec3(0.5f,  0.5f, -0.5f),
             glm::vec3(0.5f, -0.5f, 0.5f),
             glm::vec3(0.5f, -0.5f, -0.5f));

    pushFace(glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f, 0.5f));
}


#include "cubemesh.h"

void CubeMesh::pushTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
    int i0;
    int i1;
    int i2;

    glm::vec3 e01 = v0 - v1;
    glm::vec3 e02 = v0 - v2;
    glm::vec3 normal = glm::cross(e01, e02);

    if (visited.find(std::make_tuple(v0.x, v0.y, v0.z)) != visited.end()) {
        i0 = visited[std::make_tuple(v0.x, v0.y, v0.z)];
    } else {
        i0 = this->pushVertex(v0, normal); // this normalization is unnecessary
        visited[std::make_tuple(v0.x, v0.y, v0.z)] = i0;
    }

    if (visited.find(std::make_tuple(v1.x, v1.y, v1.z)) != visited.end()) {
        i1 = visited[std::make_tuple(v1.x, v1.y, v1.z)];
    } else {
        i1 = this->pushVertex(v1, normal); // this normalization is unnecessary
        visited[std::make_tuple(v1.x, v1.y, v1.z)] = i1;
    }

    if (visited.find(std::make_tuple(v2.x, v2.y, v2.z)) != visited.end()) {
        i2 = visited[std::make_tuple(v2.x, v2.y, v2.z)];
    } else {
        i2 = this->pushVertex(v2, normal); // this normalization is unnecessary
        visited[std::make_tuple(v2.x, v2.y, v2.z)] = i2;
    }

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


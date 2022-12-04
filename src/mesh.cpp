#include "mesh.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

int Mesh::pushVertex(glm::vec3 v, glm::vec3 vn)
{
    vertices.push_back(v.x);
    vertices.push_back(v.y);
    vertices.push_back(v.z);
    vertices.push_back(vn.x);
    vertices.push_back(vn.y);
    vertices.push_back(vn.z);
    this->numVertices++;
    return numVertices - 1;
}

glm::vec3 Mesh::getVertex(int i) {
    return glm::vec3(vertices[i * 6], vertices[i * 6 + 1], vertices[i * 6 + 2]);
}

glm::vec3 Mesh::getNormal(int i) {
    return glm::vec3(vertices[i * 6 + 3], vertices[i * 6 + 4], vertices[i * 6 + 5]);
}

void Mesh::setVertex(int i, glm::vec3 pos) {
    this->vertices[i * 6] = pos.x;
    this->vertices[i * 6 + 1] = pos.y;
    this->vertices[i * 6 + 2] = pos.z;
}

void Mesh::setNormal(int i, glm::vec3 normal) {
    this->vertices[i * 6 + 3] = normal.x;
    this->vertices[i * 6 + 4] = normal.y;
    this->vertices[i * 6 + 5] = normal.z;
}

void Mesh::buildGeometry(glm::mat4 model) {
    glm::mat4 normalMatrix = glm::mat4(glm::inverse(glm::transpose(glm::mat3(model))));
    for (int i = 0; i < numVertices; i++) {
        setVertex(i, glm::vec3(model * glm::vec4(getVertex(i), 1)));
        setNormal(i, glm::vec3(normalMatrix * glm::vec4(getNormal(i), 0)));
    }
}

void Mesh::genBuffers() {
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ebo);
    glGenVertexArrays(1, &this->vao);
}

void Mesh::initBuffers() {
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    // add data vbo buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int), this->indices.data(), GL_STATIC_DRAW);
    // load data into vao
    // vertices
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), reinterpret_cast<void *>(0));
    // normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), reinterpret_cast<void *>(3 * sizeof(GL_FLOAT)));
    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::deleteBuffers() {
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->ebo);
}

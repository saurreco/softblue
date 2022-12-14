#include "cubemesh.h"

void CubeMesh::pushTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
}

void CubeMesh::pushFace(glm::vec3 upLeft, glm::vec3 upRight, glm::vec3 downLeft, glm::vec3 downRight) {
}

void CubeMesh::transform(glm::mat4 model) {
    modelMatrix = model;
    for (int i = 0; i < numVertices; i++) {
         setVertex(i, glm::vec3(model * glm::vec4(getVertex(i), 1)));
    }
    updateBuffers();
}

void CubeMesh::createCube() {

    vertices = std::vector<float>{
        0.5, 0.5, 0.5,      0, 0, 1,
        -0.5, 0.5, 0.5,     0, 0, 1,
        0.5, -0.5, 0.5,     0, 0, 1,
        -0.5, -0.5, 0.5,    0, 0, 1,

        -0.5, 0.5, -0.5,    0, 1, 0,
        -0.5, 0.5, 0.5,     0, 1, 0,
        0.5, 0.5, 0.5,      0, 1, 0,
        0.5, 0.5, -0.5,     0, 1, 0,

        0.5, 0.5, -0.5,     0, 0, -1,
        -0.5, -0.5, -0.5,   0, 0, -1,
        -0.5, 0.5, -0.5,    0, 0, -1,
        0.5, -0.5, -0.5,    0, 0, -1,

        0.5, -0.5, -0.5,    0, -1, 0,
        0.5, -0.5, 0.5,     0, -1, 0,
        -0.5, -0.5, 0.5,    0, -1, 0,
        -0.5, -0.5, -0.5,   0, -1, 0,

        0.5, 0.5, 0.5,      1, 0, 0,
        0.5, -0.5, 0.5,     1, 0, 0,
        0.5, -0.5, -0.5,    1, 0, 0,
        0.5, 0.5, -0.5,     1, 0, 0,

        -0.5, 0.5, 0.5,     -1, 0, 0,
        -0.5, 0.5, -0.5,    -1, 0, 0,
        -0.5, -0.5, 0.5,    -1, 0, 0,
        -0.5, -0.5, -0.5,   -1, 0, 0
    };

    numVertices = 24;

    indices = std::vector<int>{
        0, 1, 2,
        1, 3, 2,

        4, 5, 6,
        4, 6, 7,

        8, 9, 10,
        8, 11, 9,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        21, 23, 22
    };
    numIndices = 12 * 3;
}

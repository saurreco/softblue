#include "tetmesh.h"

void TetMesh::createTet() {

    float sqrtHalf = sqrt(1 / 2);

    vertices = std::vector<float>{1, 0, -sqrtHalf, 1, 0, -sqrtHalf,
                                  -1, 0, -sqrtHalf, -1, 0, -sqrtHalf,
                                  0, 1, sqrtHalf, 0, 1, sqrtHalf,
                                  0, -1, sqrtHalf, 0, -1, sqrtHalf};
    numVertices = 4;
    indices = std::vector<int>{2, 3, 0,
                               2, 0, 1,
                               2, 1, 3,
                               1, 3, 0};
    numIndices = 12;
}
void TetMesh::pushTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {};


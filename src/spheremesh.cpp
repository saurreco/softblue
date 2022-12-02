#include "spheremesh.h"
#include <tuple>

/**
 * @brief add a triangle to vertex buffer, where vertices 0, 1 & 2 follow a counterclockwise order
 *
 * @param v0
 * @param v1
 * @param v2
 */

void SphereMesh::pushTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
    int i0 = this->pushVertex(v0, glm::normalize(v0));
    int i1 = this->pushVertex(v1, glm::normalize(v1));
    int i2 = this->pushVertex(v2, glm::normalize(v2));
    this->indices.push_back(i0);
    this->indices.push_back(i1);
    this->indices.push_back(i2);
}

void SphereMesh::createIcosahedron()
{
    /*
    float phi = (1.0f + sqrt(5.0f)) * 0.5f; // golden ratio
    float a = 1.0f;
    float b = 1.0f / phi;

    glm::vec3 v1 = glm::normalize(glm::vec3(0, b, -a));
    glm::vec3 v2 = glm::normalize(glm::vec3(b, a, 0));
    glm::vec3 v3 = glm::normalize(glm::vec3(-b, a, 0));
    glm::vec3 v4 = glm::normalize(glm::vec3(0, b, a));
    glm::vec3 v5 = glm::normalize(glm::vec3(0, -b, a));
    glm::vec3 v6 = glm::normalize(glm::vec3(-a, 0, b));
    glm::vec3 v7 = glm::normalize(glm::vec3(0, -b, -a));
    glm::vec3 v8 = glm::normalize(glm::vec3(a, 0, -b));
    glm::vec3 v9 = glm::normalize(glm::vec3(a, 0, b));
    glm::vec3 v10 = glm::normalize(glm::vec3(-a, 0, -b));
    glm::vec3 v11 = glm::normalize(glm::vec3(b, -a, 0));
    glm::vec3 v12 = glm::normalize(glm::vec3(-b, -a, 0));

    this->pushTriangle(v3, v2, v1);
    this->pushTriangle(v2, v3, v4);
    this->pushTriangle(v6, v5, v4);
    this->pushTriangle(v5, v9, v4);
    this->pushTriangle(v8, v7, v1);
    this->pushTriangle(v7, v10, v1);
    this->pushTriangle(v12, v11, v5);
    this->pushTriangle(v11, v12, v7);
    this->pushTriangle(v10, v6, v3);
    this->pushTriangle(v6, v10, v12);
    this->pushTriangle(v9, v8, v2);
    this->pushTriangle(v8, v9, v11);
    this->pushTriangle(v3, v6, v4);
    this->pushTriangle(v9, v2, v4);
    this->pushTriangle(v10, v3, v1);
    this->pushTriangle(v2, v8, v1);
    this->pushTriangle(v12, v10, v7);
    this->pushTriangle(v8, v11, v7);
    this->pushTriangle(v6, v12, v5);
    this->pushTriangle(v11, v9, v5);
    */
    std::vector<float> tmpVerts = {-1.0f, 1.0f, 0.0f,
                                   0.0f, 0.0f, 0.0f,
                                   -1.0f, -1.0f, 0.0f,
                                   0.0f, 0.0f, 0.0f, 0.0f,
                                   1.0f, -1.0f, 0.0f,
                                   0.0f, 0.0f, 0.0f,
//                                   1.f,  1.f, 0.0f,
//                                   0.0f, 0.0f, 0.0f,
                                   -1.f,  1.f, 0.0f,
                                   0.0f, 0.0f, 0.0f,
                                   1.f, -1.f, 0.0f,
                                   0.0f, 0.0f, 0.0f};
    std::vector<int> tmpIndices = {0, 1, 2, 3, 0, 4};
    vertices = tmpVerts;
    indices = tmpIndices;
    numIndices = 6;
    numVertices = 5;
}

void SphereMesh::subdivide()
{
    int numTriangles = this->indices.size() / 3;
    std::vector<int> tmpIndices;
    for (int i = 0; i < numTriangles; i++)
    { /* loop through triangles */
        std::vector<int> tmp;
        for (int j = 0; j < 3; j++)
        { /* loop through edges */
            int l = i + j;
            int r = i + (j + 1 % 3);
            if (l > r)
            {
                int tmp = l;
                l = r;
                r = tmp;
            }
            std::pair<int, int> e = std::make_pair(l, r);
            if (edgeMap.find(e) != edgeMap.end())
            {
                tmp.push_back(edgeMap[e]);
            }
            else
            {
                glm::vec3 v0(this->vertices[l],
                             this->vertices[l + 1],
                             this->vertices[l + 2]);
                glm::vec3 v1(this->vertices[r],
                             this->vertices[r + 1],
                             this->vertices[r + 2]);
                glm::vec3 d = v1 - v0;
                glm::vec3 v = v0 + 0.5f * d;
                glm::vec3 vn = normalize(v);
                int idx = pushVertex(v, vn);
                tmp.push_back(idx);
                edgeMap[e] = idx;
            }
        }

        int v0 = i;
        int v1 = i + 1;
        int v2 = i + 2;
        int i0 = tmp[0];
        int i1 = tmp[1];
        int i2 = tmp[2];

        tmpIndices.push_back(v0);
        tmpIndices.push_back(i0);
        tmpIndices.push_back(i2);

        tmpIndices.push_back(i0);
        tmpIndices.push_back(v1);
        tmpIndices.push_back(i1);

        tmpIndices.push_back(i0);
        tmpIndices.push_back(i1);
        tmpIndices.push_back(i2);

        tmpIndices.push_back(i2);
        tmpIndices.push_back(i1);
        tmpIndices.push_back(v2);
    }

    this->indices.clear();
    for (int i : tmpIndices)
        this->indices.push_back(i);
}

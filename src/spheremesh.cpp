#include "spheremesh.h"
#include <tuple>
#include <glm/gtx/string_cast.hpp>

void SphereMesh::pushTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
    int i0 = this->pushVertex(v0, glm::normalize(v0)); // this normalization is unnecessary
    int i1 = this->pushVertex(v1, glm::normalize(v1));
    int i2 = this->pushVertex(v2, glm::normalize(v2));
    this->indices.push_back(i0);
    this->indices.push_back(i1);
    this->indices.push_back(i2);
    numIndices += 3;
}

void SphereMesh::createIcosahedron()
{
    float phi = (1.0f + sqrt(5.0f)) * 0.5f; // golden ratio
    float a = 1.0f;
    float b = 1.0f / phi;

    // SPHERE OF RADIUS 1
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


}

void SphereMesh::subdivide()
{
    std::map<std::pair<int, int>, int> edgeMap;
    numIndices = 0;
    int numTriangles = this->indices.size() / 3;
    std::vector<int> tmpIndices;
    for (int i = 0; i < numTriangles; i++)
    { /* loop through triangles */
        std::vector<int> tmp;
        for (int j = 0; j < 3; j++)
        { /* loop through edges */
            int l = i * 3 + j;
            int r = i * 3 + (j + 1) % 3;
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
                glm::vec3 v0 = getVertex(indices[l]);
                glm::vec3 v1 = getVertex(indices[r]);
                glm::vec3 v = 0.5f * (v0 + v1);

                glm::vec3 vn = normalize(v);
                int idx = pushVertex(vn, vn);
                tmp.push_back(idx);
                edgeMap[e] = idx;
            }
        }

        int v0 = this->indices[i * 3];
        int v1 = this->indices[i * 3 + 1];
        int v2 = this->indices[i * 3 + 2];
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

        numIndices += 12;
    }

    this->indices.clear();
    for (int i : tmpIndices)
        this->indices.push_back(i);
}

#pragma once
#include "mesh.h"
#include <vector>
#include <glm/glm.hpp>
#include <tuple>
#include <map>
#include <utility>
#include <GL/glew.h>
#include "debug.h"

class SphereMesh : public Mesh
{
public:
    SphereMesh() {
        createIcosahedron();
//        subdivide();
        genBuffers();
        initBuffers();
    }
    void pushTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
    void subdivide();
    void createSphere();
    void createIcosahedron();

private:
    std::map<std::pair<int, int>, int> edgeMap; // pair = (left, right)
};

#pragma once

#include "mesh.h"
#include <vector>
#include <glm/glm.hpp>
#include <tuple>
#include <map>
#include <utility>
#include <GL/glew.h>

class SphereMesh : public Mesh
{
public:
    SphereMesh(glm::mat4 model) {
        createIcosahedron();
        subdivide();
        subdivide();
        buildGeometry(model);
        genBuffers();
        initBuffers();
    }
    SphereMesh() {
        createIcosahedron();
        subdivide();
        subdivide();
        buildGeometry(glm::mat4(1));
        genBuffers();
        initBuffers();
    }
    void pushTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
    void createIcosahedron();
    void subdivide();

private:
};

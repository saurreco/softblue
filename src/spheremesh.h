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
    SphereMesh(int n, glm::mat4 model) {
        createIcosahedron();
       for (int i = 0; i < n; i++)
           subdivide();

        buildGeometry(model);
        genBuffers();
        initBuffers();
    }
    SphereMesh() {
        createIcosahedron();
       // subdivide();
        //subdivide();
        buildGeometry(glm::mat4(1));
        genBuffers();
        initBuffers();
    }
    std::map<std::tuple<float, float, float>, int> visited;

    void pushTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
    void createIcosahedron();
    void subdivide();

private:
};

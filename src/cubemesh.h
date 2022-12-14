#pragma once

#include "mesh.h"
#include <vector>
#include <glm/glm.hpp>
#include <map>
#include <utility>
#include <GL/glew.h>

class CubeMesh : public Mesh
{
public:
    std::map<std::tuple<float, float, float>, int> visited;
    CubeMesh() {
        createCube();
        buildGeometry(glm::mat4(1));
        genBuffers();
        initBuffers();
    }
    CubeMesh(glm::mat4 model) {
        createCube();
        buildGeometry(model);
        genBuffers();
        initBuffers();
    }
    void pushTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
    void pushFace(glm::vec3 upLeft, glm::vec3 upRight, glm::vec3 downLeft, glm::vec3 downRight);
    void createCube();

private:
};

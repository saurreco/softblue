#pragma once

#include "mesh.h"
#include <vector>
#include <glm/glm.hpp>
#include <map>
#include <utility>
#include <GL/glew.h>

class TetMesh : public Mesh
{
public:
    std::map<std::tuple<float, float, float>, int> visited;
    TetMesh() {
        createTet();
        buildGeometry(glm::mat4(1));
        genBuffers();
        initBuffers();
    }
    TetMesh(glm::mat4 model) {
        createTet();
        buildGeometry(model);
        genBuffers();
        initBuffers();
    }
    void pushTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
    void pushFace(glm::vec3 upLeft, glm::vec3 upRight, glm::vec3 downLeft, glm::vec3 downRight);
    void createTet();
private:
};

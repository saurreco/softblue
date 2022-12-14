#pragma once

#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>
#include <math.h>


struct IntersectData {
    int collide;
    glm::vec3 pos;
    glm::vec3 normal;
};

namespace Raycast {
    IntersectData cubeCollide(glm::vec3 point);
}

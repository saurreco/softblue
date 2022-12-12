
#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>
#include <math.h>
#include <vector>
#include "raycast.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/gtx/string_cast.hpp>

static float plane(glm::vec3 p, glm::vec3 d, int axis, float offset) {
    int sign = offset > 0 ? 1 : -1;
    float t = 0;
    if (d[axis] == 0)
        t = std::numeric_limits<float>::max();
    t = (-p[axis] + offset) / d[axis];
    return t;
}

struct IntersectData Raycast::cubeCollide(glm::vec3 point) {
    int hits = 0;
/*
    for (int i1 = 0; i1 < 3; i1++) {
        int i2 = (i1 + 1) % 3;
        int i3 = (i1 + 2) % 3;
        for (int i = 0; i < 2; i++) {
            int sign = i == 0 ? 1 : -1;
            float t = plane(p, d, i1, 0.5 * sign);
            glm::vec3 plane = p + t * d;
            if (plane[i2] <= 0.5 && plane[i2] >= -0.5 &&
                plane[i3] <= 0.5 && plane[i3] >= -0.5)
                hits++;
        }
    }

    return IntersectData{hits % 2, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)};
    */

    int collide = point.x <= 0.5 && point.x >= -0.5 &&
                  point.y <= 0.5 && point.y >= -0.5 &&
                  point.z <= 0.5 && point.z >= -0.5;
    if (collide) {
        std::vector<IntersectData> tmp;
        glm::vec3 top(point.x, 0.5, point.z);
        glm::vec3 bottom(point.x, -0.5, point.z);
        glm::vec3 left(-0.5, point.y, point.z);
        glm::vec3 right(0.5, point.y, point.z);
        glm::vec3 forward(point.x, point.y, 0.5);
        glm::vec3 back(point.x, point.y, -0.5);
        tmp.push_back(IntersectData{1, top, glm::vec3(0, 1, 0)});
        tmp.push_back(IntersectData{1, bottom, glm::vec3(0, -1, 0)});
        tmp.push_back(IntersectData{1, left, glm::vec3(-1, 0, 0)});
        tmp.push_back(IntersectData{1, right, glm::vec3(1, 0, 0)});
        tmp.push_back(IntersectData{1, forward, glm::vec3(0, 0, 1)});
        tmp.push_back(IntersectData{1, back, glm::vec3(0, 0, -1)});
        float dist = 10;
        IntersectData ret = tmp[0];
        for (IntersectData data : tmp) {
            float newDist = glm::length(point - data.pos);
            if (newDist < dist) {
                dist = newDist;
                ret = data;
            };
        }
        return ret;

    }
    return IntersectData{0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)};
}

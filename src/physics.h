#pragma once
#include "scene.h"

struct MassPoint {
    glm::vec3 r;
    glm::vec3 v;
    glm::vec3 f;
    float m;
};

struct Body {
    std::vector<MassPoint> masses;
};

class Physics {
public:
    void init(Scene* scene);
    void update(float dt);
    Scene* scene;
    std::vector<Body> bodies;
private:
    void applyPhysics(float dt);
    void updateScene();
};

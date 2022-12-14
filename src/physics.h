#pragma once
#include "scene.h"

struct MassPoint {
    glm::vec3 r;
    glm::vec3 v;
    glm::vec3 f;
    glm::vec3 n;
};

struct Spring {
    int A, B;
    float L;  /* rest length */
};

struct Body {
    std::vector<MassPoint> masses;
    std::vector<Spring> springs;
};

class Physics {
public:
    void init(Scene* scene);
    void update(float dt);
    Scene* scene;
    std::vector<Body> bodies;
    std::vector<Model> obstacles;
    float stiffness;
    float damping;
    float massOfPoint;
    glm::vec3 objectCenter = glm::vec3(0.f);
private:
    void applyPhysics(float dt);
    void updateScene();
};

#pragma once
#include "scene.h"

struct MassPoint {
    glm::vec3 r;
    glm::vec3 v;
    glm::vec3 f;
};

struct Spring {
    int A, B;
    float L;  /* rest length */
};

struct Body {
    std::vector<MassPoint> masses;
    std::vector<Spring> springs;
    glm::vec3 center;
    float k;
    float p;
};

class Physics {
public:
    void init(Scene* scene);
    void update(float dt);
    void calcCenter(Body& body);
    Scene* scene;
    std::vector<Body> bodies;
    std::vector<Model> obstacles;
    float stiffness;
    float damping;
    float massOfPoint;
        void updateScene();
private:
    void applyPhysics(float dt);

};

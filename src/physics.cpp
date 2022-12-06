#include "physics.h"

void Physics::init(Scene* scene) {
    bodies.clear();
    for (Model model : scene->models) {
        Body body;
        for (int i = 0; i < model.geometry->numVertices; i++) {
            MassPoint mass;
            mass.m = 0.1f;
            mass.v = glm::vec3(0, 0, 0);
            mass.f = glm::vec3(0, 0, 0);
            mass.r = model.geometry->getVertex(i);
            body.masses.push_back(mass);
        }
        bodies.push_back(body);
    }
    this->scene = scene;
}

void Physics::update(float dt) {
    applyPhysics(dt);
    updateScene();
}

void Physics::applyPhysics(float dt) {

    for (Body body : bodies) {
        for (MassPoint mass : body.masses) {
            mass.f = glm::vec3(0, 0, 0);
            mass.f += -9.8f * mass.m;
            mass.v += (mass.f * dt) / mass.m;
            mass.r += mass.v * dt;
        }
    }
}

void Physics::updateScene() {
    for (int i = 0; i < bodies.size(); i++) {
        for (int j = 0; j < bodies[i].masses.size(); j++) {
            scene->models[i].geometry->setVertex(j, bodies[i].masses[j].r);
        }
    }
}

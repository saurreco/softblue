#include "physics.h"

void Physics::init(Scene* scene) {
    bodies.clear();
    for (Model model : scene->models) {
        Body body;
        for (int i = 0; i < model.geometry->numVertices; i++) {
            MassPoint mass;
            mass.m = 1.0f;
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

    for (Body& body : bodies) {

        for (MassPoint& mass : body.masses) {

            mass.f = glm::vec3(0, 0, 0);
            mass.f += glm::vec3(0, -9.8f, 0) * mass.m; // force accumulation
            mass.v += (mass.f * dt) / mass.m;
            mass.r += mass.v * dt; // integration

            /* collision */
            if (mass.r.y <= -5.0f) {
                mass.r.y = -5.0f;
                mass.v = 0.5f * glm::reflect(mass.v, glm::vec3(0, 1, 0));
            }
        }
    }
}

void Physics::updateScene() {
    for (int i = 0; i < bodies.size(); i++) {
        for (int j = 0; j < bodies[i].masses.size(); j++) {
            scene->models[i].geometry->setVertex(j, bodies[i].masses[j].r);
            //scene->models[i].geometry->setNormal(j, glm::normalize(bodies[i].masses[j].r));
        }
        scene->models[i].geometry->updateBuffers();
    }
}

#include "physics.h"
#include <tuple>
#include <map>

void Physics::init(Scene* scene) {
    bodies.clear();

    for (Model model : scene->models) {
        Body body;


        /* mass point */
        for (int i = 0; i < model.geometry->numVertices; i++) {
            MassPoint mass;
            mass.m = 1.0f;
            mass.v = glm::vec3(0, 0, 0);
            mass.f = glm::vec3(0, 0, 0);
            mass.r = model.geometry->getVertex(i);
            body.masses.push_back(mass);
        }

        /* spring */
        std::map<std::pair<int, int>, int> edgeMap;
        for (int i = 0; i < model.geometry->numIndices / 3; i++) {
            for (int j = 0; j < 3; j++) {
                int a = i * 3 + j;
                int b = i * 3 + ((j + 1) % 3);

                if (a > b) {
                    int tmp = b;
                    b = a;
                    a = tmp;
                }

                std::pair<int, int> e = std::make_pair(a, b);
                if (edgeMap.find(e) == edgeMap.end()) {
                    Spring spring;
                    spring.A = model.geometry->indices[a];
                    spring.B = model.geometry->indices[b];
                    glm::vec3 v0 = model.geometry->getVertex(spring.A);
                    glm::vec3 v1 = model.geometry->getVertex(spring.B);
                    spring.L = glm::length(v0 - v1);
                    body.springs.push_back(spring);
                }
            }
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
            mass.f += glm::vec3(0, -9.8f, 0) * mass.m;
        }

        for (Spring& spring : body.springs) {
            MassPoint& massA = body.masses[spring.A];
            MassPoint& massB = body.masses[spring.B];
            float fs = 50.0f * (glm::length(massB.r - massA.r) - spring.L);
            glm::vec3 AtoB = glm::normalize(massB.r - massA.r);
            glm::vec3 BtoA = glm::normalize(massA.r - massB.r);
            float fta = fs + glm::dot(AtoB, massB.v - massA.v) * 0.5;
            float ftb = fs + glm::dot(BtoA, massA.v - massB.v) * 0.5;
            massA.f += AtoB * fta;
            massB.f += BtoA * ftb;
        }

        for (MassPoint& mass : body.masses) {
            mass.v += (mass.f * dt) / mass.m;
            mass.r += mass.v * dt; // integration

            /* collision */
            if (mass.r.y <= -5.0f) {
                mass.r.y = -5.0f;
                mass.v = 0.3f * glm::reflect(mass.v, glm::vec3(0, 1, 0));
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

#include "physics.h"
#include "raycast.h"
#include <tuple>
#include <map>

void Physics::init(Scene* scene) {
    bodies.clear();
    massOfPoint = 1.0f;

    for (Model model : scene->models) {
        if (!model.type) {
            obstacles.push_back(model);
            continue;
        }

        Body body;

        /* mass point */
        for (int i = 0; i < model.geometry->numVertices; i++) {
            MassPoint mass;
            mass.v = glm::vec3(0, 0, 0);
            mass.f = glm::vec3(0, 0, 0);
            mass.r = model.geometry->getVertex(i);
            mass.n = model.geometry->getNormal(i);
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
    int i = 0;
    for (Body& body : bodies) {

        for (MassPoint& mass : body.masses) {
            mass.f = glm::vec3(0, 0, 0);
            mass.f += glm::vec3(0, -9.8f, 0) * massOfPoint;
        }

        for (Spring& spring : body.springs) {
            MassPoint& massA = body.masses[spring.A];
            MassPoint& massB = body.masses[spring.B];
            float fs = 1000.0f * (glm::length(massB.r - massA.r) - spring.L);
            glm::vec3 dir = massB.r - massA.r;
            float fd = 500.0f * glm::dot(dir, massB.v - massA.v);
            massA.f += (fs + fd) * dir;
            massB.f -= (fs + fd) * dir;
        }


        float bottom = 1000;
        float top = -1000;
        float left = 1000;
        float right = -1000;
        float near = -1000;
        float far = 1000;
        for (MassPoint& mass : body.masses) {
            if (mass.r.x < left)
                left = mass.r.x;
            if (mass.r.x > right)
                right = mass.r.x;
            if (mass.r.y < bottom)
                bottom = mass.r.y;
            if (mass.r.y > top)
                top = mass.r.y;
            if (mass.r.z > near)
                near = mass.r.z;
            if (mass.r.z < far)
                far = mass.r.z;
        }
        glm::vec3 center(0, 0, 0);
        for (MassPoint mass : body.masses) {
            center += mass.r;

        }
        center = center * (1 / (float)body.masses.size());
//        float obj_x = (right - left) / 2 + right;
//        float obj_y = (top - bottom) / 2 + top;
//        float obj_z = (far - near) / 2 + far;
        this->objectCenter = center;


        for (int k = 0; k < scene->models[i].geometry->numIndices / 3; k++) {

            MassPoint& v0 = body.masses[scene->models[i].geometry->indices[k * 3]];
            MassPoint& v1 = body.masses[scene->models[i].geometry->indices[k * 3 + 1]];
            MassPoint& v2 = body.masses[scene->models[i].geometry->indices[k * 3 + 2]];
            glm::vec3 e01 = v1.r - v0.r;
            glm::vec3 e02 = v2.r - v0.r;
            float area = glm::length(glm::cross(e01, e02));
            glm::vec3 normal = glm::normalize(glm::cross(e01, e02));
            float V = abs((right - left) * (top - bottom) * (far - near));
            float P = 1000.0;
            float f = ((area) * P) / V;
            v0.f += f * normal;
            v1.f += f * normal;
            v2.f += f * normal;
            v0.n = normal;
            v1.n = normal;
            v2.n = normal;

        }

        for (MassPoint& mass : body.masses) {
            mass.v += (mass.f * dt) / massOfPoint;
            mass.r += mass.v * dt; // integration


            /* collision */
            for (Model model : obstacles) {
                glm::mat4 modelMatrix = model.geometry->modelMatrix;
                glm::mat4 inverseModelMatrix =  model.geometry->inverseModelMatrix;
                glm::mat4 normalMatrix = model.geometry->normalMatrix;
                glm::vec3 objectR = glm::vec3(inverseModelMatrix * glm::vec4(mass.r, 1));
                IntersectData hit = Raycast::cubeCollide(objectR);
                if (hit.collide) {
                    mass.r = modelMatrix * glm::vec4(hit.pos, 1);
                    mass.v = 0.5f * glm::reflect(mass.v, glm::vec3(normalMatrix * glm::vec4(hit.normal, 0)));
                }
            }
        }
        i++;
    }
}

void Physics::updateScene() {
    for (int i = 0; i < bodies.size(); i++) {
        for (int j = 0; j < bodies[i].masses.size(); j++) {
            scene->models[i].geometry->setVertex(j, bodies[i].masses[j].r);
           scene->models[i].geometry->setNormal(j,  glm::normalize(bodies[i].masses[j].r - objectCenter)); // glm::normalize(bodies[i].masses[j].r - objectCenter)
        }
        scene->models[i].geometry->updateBuffers();
    }
}

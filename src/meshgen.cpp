
#include "meshgen.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "glm/gtc/constants.hpp"
#include "glm/gtx/transform.hpp"

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

void insertVec2(std::vector<float> &data, glm::vec2 v) {
    data.push_back(v.x);
    data.push_back(v.y);
}


void makeTile(glm::vec3 topLeft,
              glm::vec3 topRight,
              glm::vec3 bottomLeft,
              glm::vec3 bottomRight,
              std::vector<GLfloat> &data,
              std::function<glm::vec3(glm::vec3)> getNormal) {
    // Task 5: Implement the makeTile() function for a Sphere
    // Note: this function is very similar to the makeTile() function for Cube,
    //       but the normals are calculated in a different way!
    // triangle 1
    insertVec3(data, topLeft);
    insertVec3(data, getNormal(topLeft));
    insertVec2(data, glm::vec2(0, 0));

    insertVec3(data, bottomLeft);
    insertVec3(data, getNormal(bottomLeft));
    insertVec2(data, glm::vec2(0, 0));

    insertVec3(data, bottomRight);
    insertVec3(data, getNormal(bottomRight));
    insertVec2(data, glm::vec2(0, 0));

    // triangle 2
    insertVec3(data, topRight);
    insertVec3(data, getNormal(topRight));
    insertVec2(data, glm::vec2(0, 0));

    insertVec3(data, topLeft);
    insertVec3(data, getNormal(topLeft));
    insertVec2(data, glm::vec2(0, 0));

    insertVec3(data, bottomRight);
    insertVec3(data, getNormal(bottomRight));
    insertVec2(data, glm::vec2(0, 0));

}

glm::vec3 toXYZ(float phi, float theta) {
    float r = 0.5;
    float x = r * glm::sin(phi) * glm::sin(theta);
    float y = r * glm::cos(phi);
    float z = r * glm::sin(phi) * glm::cos(theta);
    return glm::vec3(x, y, z);
}

void makeCubeFace(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight,
                  int p1,
                  int p2,
                  std::vector<GLfloat> &data) {
    glm::vec3 u = glm::normalize(topRight - topLeft);
    glm::vec3 v = -glm::normalize(bottomLeft - topLeft);
    glm::vec3 normal = glm::normalize(glm::cross(u, v));
    float s = 1 / (float) p1;
    glm::vec3 stepU = s * u;
    glm::vec3 stepV = s * v;
    float stepSize = (float)(p1 - 1);
    bottomLeft = bottomLeft + stepSize * stepV;
    bottomRight = bottomRight + stepSize * stepV - stepSize * stepU;
    topRight = topRight - stepSize * stepU;
    for (int i = 0; i < p1; i++) {
        for (int j = 0; j < p1; j++) {
            float step = (float)j;
            makeTile(topLeft + step * stepU,
                     topRight + step * stepU,
                     bottomLeft + step * stepU,
                     bottomRight + step * stepU,
                     data,
                     [normal](glm::vec3 pos) -> glm::vec3 { return normal; } );
        }
        topLeft -= stepV;
        topRight -= stepV;
        bottomLeft -= stepV;
        bottomRight -= stepV;
    }
}

std::vector<GLfloat> Meshgen::genCube(int p1, int p2) {
    std::vector<GLfloat> data;

    makeCubeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
                 glm::vec3( 0.5f,  0.5f, 0.5f),
                 glm::vec3(-0.5f, -0.5f, 0.5f),
                 glm::vec3( 0.5f, -0.5f, 0.5f),
                 p1, p2, data);

    makeCubeFace(glm::vec3( 0.5f,  0.5f, -0.5f),
                 glm::vec3(-0.5f,  0.5f, -0.5f),
                 glm::vec3( 0.5f, -0.5f, -0.5f),
                 glm::vec3(-0.5f, -0.5f, -0.5f),
                 p1, p2, data);

   makeCubeFace(glm::vec3( 0.5f, -0.5f, -0.5f),
                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3( 0.5f, -0.5f, 0.5f),
                glm::vec3(-0.5f, -0.5f, 0.5f),
                p1, p2, data);

   makeCubeFace(glm::vec3(-0.5f, 0.5f, -0.5f),
                glm::vec3( 0.5f, 0.5f, -0.5f),
                glm::vec3(-0.5f, 0.5f, 0.5f),
                glm::vec3( 0.5f, 0.5f, 0.5f),
                p1, p2, data);

   makeCubeFace(glm::vec3(0.5f,  0.5f, 0.5f),
                glm::vec3(0.5f,  0.5f, -0.5f),
                glm::vec3(0.5f, -0.5f, 0.5f),
                glm::vec3(0.5f, -0.5f, -0.5f),
                p1, p2, data);

   makeCubeFace(glm::vec3(-0.5f,  0.5f, -0.5f),
                glm::vec3(-0.5f,  0.5f, 0.5f),
                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3(-0.5f, -0.5f, 0.5f),
                p1, p2, data);

   return data;
}

std::vector<GLfloat> Meshgen::genSphere(int p1, int p2) {
    p1 = fmax(p1, 2);
    p2 = fmax(p2, 3);
    std::vector<GLfloat> data;

    float pi = 3.14159;

    float theta = 0;
    float stepTheta = (2 * pi) / (float)p2;
    float nextTheta = theta + stepTheta;

    for (int i = 0; i < p2; i++) {

        float phi = 0;
        float stepPhi = pi / (float)p1;
        float nextPhi = phi + stepPhi;

        for (int i = 0; i < p1; i++) {
            makeTile(toXYZ(phi, theta),
                     toXYZ(phi, nextTheta),
                     toXYZ(nextPhi, theta),
                     toXYZ(nextPhi, nextTheta),
                     data,
                     [](glm::vec3 pos) -> glm::vec3 { return pos; });
            phi = nextPhi;
            nextPhi += stepPhi;
        }


        theta = nextTheta;
        nextTheta += stepTheta;
    }

    return data;
}

glm::vec3 toXYZ(float theta, glm::vec3 u, glm::vec3 v) {
    float r = 0.5;
    float x = r * glm::cos(theta);
    float z = r * glm::sin(theta);
    return x * u + z * v;
}

void makeCap(glm::vec3 normal, int p1, int p2, std::vector<GLfloat>& data) {
    float pi = 3.14159;
    glm::vec3 u(0, 0, 1);
    glm::vec3 v = glm::normalize(glm::cross(u, normal));
    float stepSize = (2 * pi) / (float)p2;
    float theta = 0.5f * stepSize;
    float nextTheta = theta + stepSize;
    for (int i = 0; i < p2; i++) {
        glm::vec3 left = toXYZ(theta, u, v) ;
        glm::vec3 right = toXYZ(nextTheta, u, v) ;
        for (int j = 0; j < p1; j++) {
            float alpha = j / (float)p1;
            float beta = (j + 1) / (float)p1;
           makeTile(alpha * left + -0.5f * normal,
                   alpha * right + -0.5f * normal,
                   beta * left + -0.5f * normal,
                   beta * right + -0.5f * normal,
                   data,
                   [normal] (glm::vec3 pos) -> glm::vec3 {return -normal;});
        }
        theta = nextTheta;
        nextTheta += stepSize;
    }
}

std::vector<GLfloat> Meshgen::genCylinder(int p1, int p2) {
    std::vector<GLfloat> data;
    p1 = fmax(p1, 1);
    p2 = fmax(p2, 3);
    makeCap(glm::vec3(0, -1, 0), p1, p2, data);
    makeCap(glm::vec3(0, 1, 0), p1, p2, data);

    glm::vec3 normal(0, 0, 1);
    float dtheta =  2 * 3.14159 / (float)p2;
    for (int i = 0; i < p2; i++) {

        glm::vec3 up(0, 1, 0);
        glm::vec3 u = glm::normalize(glm::cross(up, normal));
        glm::vec3 v = -glm::normalize(-up);

        float sf = 1 / (float)p1;
        float theta = 2 * 3.14159 / (float)p2;
        float widthOver2 = 0.5f * sin(theta / 2.0f);
        float depth = 0.5f * cos(theta / 2.0f);

        glm::vec3 topLeft = -widthOver2 * u + 0.5f * v + depth * normal;
        glm::vec3 topRight = widthOver2 * u + 0.5f * v + depth * normal;
        glm::vec3 bottomLeft = -widthOver2 * u + -0.5f * v + depth * normal;
        glm::vec3 bottomRight = widthOver2 * u + -0.5f * v + depth * normal;

        bottomLeft += (p1 - 1) * sf * v;
        bottomRight += (p1 - 1) * sf * v;

        for (int i = 0; i < p1; i++) {
            makeTile(topLeft,
                     topRight,
                     bottomLeft,
                     bottomRight,
                     data,
                     [] (glm::vec3 pos) -> glm::vec3 {
                return glm::vec3(pos.x, 0, pos.z);
            });
            topLeft -= sf * v;
            topRight -= sf * v;
            bottomLeft -=  sf * v;
            bottomRight -= sf * v;
        }

        normal = glm::rotateY(normal, dtheta);
    }

    return data;
}

std::vector<GLfloat> Meshgen::genCone(int p1, int p2) {
    p1 = fmax(p1, 1);
    p2 = fmax(p2, 3);
    std::vector<GLfloat> data;
    makeCap(glm::vec3(0, 1, 0), p1, p2, data);
    glm::vec3 normal(0, -1, 0);
    float pi = 3.14159;
      glm::vec3 u(0, 0, 1);
      glm::vec3 v = glm::normalize(glm::cross(u, normal));
      float stepSize = (2 * pi) / (float) p2;
      float theta = 0.5f * stepSize;
      float nextTheta = theta + stepSize;
      for (int i = 0; i < p2; i++) {
          glm::vec3 left = toXYZ(theta, u, v) ;
          glm::vec3 right = toXYZ(nextTheta, u, v) ;
          for (int j = 0; j < p1; j++) {
              float alpha = j / (float)p1;
              float beta = (j + 1) / (float)p1;
             makeTile(alpha * left - (0.5f - alpha) * normal,
                      alpha * right - (0.5f - alpha) * normal,
                      beta * left - (0.5f - beta) * normal,
                      beta * right - (0.5f - beta) * normal,
                      data,
                      [] (glm::vec3 pos) -> glm::vec3 {
             return glm::vec3( 2 * pos.x, 0.25 - 0.5f * (pos.y), 2 * pos.z);});
          }
          theta = nextTheta;
          nextTheta += stepSize;
      }
      return data;
}


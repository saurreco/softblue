#pragma once

#include "mesh.h"
#include "spheremesh.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



struct Model {
  glm::mat4 ctm;
  Mesh* geometry;
  glm::vec4 ambient;
  glm::vec4 diffuse;
  glm::vec4 specular;
  float shininess;
};

struct Light {
    glm::vec4 color;
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 function;
    float penumbra;
    float angle;
};

struct Camera {
    glm::vec3 pos;
    glm::vec3 look;
    glm::vec3 up;
    float heightAngle;
};

class Scene {
public:
    Scene(int w, int h) {
        glm::mat4 tmpProj = glm::perspective(45.0f, (w / (float)h), 0.1f, 100.0f);
        glm::mat4 tmpView = glm::lookAt(glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        mvp = tmpProj * tmpView;
    }
    void drawModel(Model model);
    void addModel(Mesh* mesh, glm::mat4 ctm);
    void addLight(glm::vec4 color, glm::vec3 pos, glm::vec3 dir);

    glm::mat4 m_view;
    glm::mat4 m_proj;
    glm::mat4 mvp;
    GLuint shader;
    Model testModel;
    void initModel();
private:
    float ka;
    float kd;
    float ks;
    Camera camera;
    Light lights[8];
    int numLights;
    std::vector<Model> models;
    int numModels;
};

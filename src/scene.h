#pragma once

#include "mesh.h"
#include "spheremesh.h""
#include <GL/glew.h>


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
    void drawModel(Model model);
    void addModel(Mesh* mesh, glm::mat4 ctm);
    void addLight(glm::vec4 color, glm::vec3 pos, glm::vec3 dir);

    glm::mat4 m_view;
    glm::mat4 m_proj;
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

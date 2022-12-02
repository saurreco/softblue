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
    glm::vec4 pos;
    glm::vec4 dir;
    glm::vec3 function;
    float penumbra;
    float angle;
};

struct Camera {
    glm::vec4 pos;
    glm::vec4 look;
    glm::vec4 up;
    float heightAngle;
};

class Scene {
public:
    void sceneInit(int w, int h);
    void matricesInit(int w, int h);
    void drawModel(Model model);
    void addModel(Mesh* mesh, glm::mat4 ctm);
    void addLight(glm::vec4 color, glm::vec4 pos, glm::vec4 dir);

    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;
    glm::mat4 mvp;
    GLuint shader;

    // current TEST scene
    Model testModel = Model();
    Light testPointLight = Light();

    void initModel();
private:
    float ka = 0.5;
    float kd = 0.5;
    float ks = 0.5;
    Camera camera;
    Light lights[8];
    int numLights;
    std::vector<Model> models;
    int numModels;

    // screen side lengths
    int screenWidth;
    int screenHeight;
};

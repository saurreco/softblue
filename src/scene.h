#pragma once

#include "mesh.h"
#include "spheremesh.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QOpenGLWidget>
#include "shader.h"
#include "camera.h"
#include "cubemap.h"

struct Model {
    Mesh* geometry;
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    bool isEnvMapped;
};

struct Light {
    glm::vec4 color;
    glm::vec3 dir;
};

class Scene {
public:
    void drawScene(Shader* shader, Camera* camera);
    void addModel(Mesh* mesh, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, bool isEnvMapped);
    void setLight(glm::vec4 color, glm::vec3 dir);
    void setupCubemap();

    std::vector<Model> models;
    int screenWidth;
    int screenHeight;

    void drawFboSide(Shader* shader, Camera* camera);

private:
    void drawModel(Shader* shader, Camera* camera, Model model);

    Light light;
    Cubemap* cubemap = new Cubemap();

    Camera* topCamera = new Camera(); // private cameras for scene, should not be put in real time!
    Camera* bottomCamera = new Camera();
    Camera* leftCamera = new Camera();
    Camera* rightCamera = new Camera();
    Camera* frontCamera = new Camera();
    Camera* backCamera = new Camera();

    void initializeSideCameras(); // UNDONE
};

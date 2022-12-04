#pragma once

#include "mesh.h"
#include "spheremesh.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QOpenGLWidget>
#include "shader.h"
#include "camera.h"

struct Model {
    Mesh* geometry;
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
};

struct Light {
    glm::vec4 color;
    glm::vec3 dir;
};

class Scene {
public:
    void drawScene(Shader* shader, Camera* camera);
    void addModel(Mesh* mesh, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular);
    void setLight(glm::vec4 color, glm::vec3 dir);

private:
    void drawModel(Shader* shader, Camera* camera, Model model);

    Light light;
    std::vector<Model> models;
    int screenWidth;
    int screenHeight;
};

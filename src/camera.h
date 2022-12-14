#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <QOpenGLWidget>
#include <iostream>

class Camera {
public:
    Camera(){
        view = glm::mat4(1);
        proj = glm::mat4(1);
        viewProj = glm::mat4(1);
    }
    void init(int w, int h, glm::vec3 pos, glm::vec3 look, glm::vec3 up, float fov, float nearPlane);
    void setFrustum(int w, int h, float fov, float nearPlane);
    void setView(glm::vec3 pos, glm::vec3 look, glm::vec3 up);
    void updateCameraPos(glm::vec3 direction);
    glm::vec3 getTranslation(Qt::Key keyDown);
    void updateUpnLook(glm::mat3 rotationMatrix);

    glm::vec3 pos;
    glm::vec3 look;
    glm::vec3 up;
    glm::mat4 viewProj;
    glm::vec3 left;
    glm::vec3 right;
private:
    int screenWidth;
    int screenHeight;
    glm::mat4 view;
    glm::mat4 proj;
};

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
class Camera {
public:
    Camera(){
        view = glm::mat4(1);
        proj = glm::mat4(1);
        viewProj = glm::mat4(1);
    }
    void init(int w, int h, glm::vec3 pos, glm::vec3 look, glm::vec3 up);
    void setFrustum(int w, int h);
    void setView(glm::vec3 pos, glm::vec3 look, glm::vec3 up);

    glm::vec3 pos;
    glm::vec3 look;
    glm::vec3 up;
    glm::mat4 viewProj;
private:
    int screenWidth;
    int screenHeight;
    glm::mat4 view;
    glm::mat4 proj;
};

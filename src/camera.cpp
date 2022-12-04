#include "camera.h"

void Camera::init(int w, int h, glm::vec3 pos, glm::vec3 look, glm::vec3 up) {
    setFrustum(w, h);
    setView(pos, look, up);
}

void Camera::setFrustum(int w, int h) {
    this->screenWidth = w;
    this->screenHeight = h;
    this->proj = glm::perspective(45.0f, (w / (float)h), 1.0f, 100.0f);
    this->viewProj = proj * view;


}

void Camera::setView(glm::vec3 pos, glm::vec3 look, glm::vec3 up) {
    this->pos = pos;
    this->look = look;
    this->up = up;
    this->view = glm::lookAt(pos, look, up);
    this->viewProj = proj * view;
}


/*

void Scene::updateUpnLook(glm::mat3 rotationMatrix) {
    this->camera.look = glm::vec4(rotationMatrix * this->camera.look, 0.f);
    this->camera.up = glm::vec4(rotationMatrix * this->camera.up, 0.f);
}

void Scene::initializeView() {
    this->viewMatrix = glm::lookAt(glm::vec3(this->camera.pos), glm::vec3(this->camera.pos + this->camera.look), glm::vec3(this->camera.up));
    this->inverseViewMatrix = glm::inverse(this->viewMatrix);
    this->left = glm::cross(glm::vec3(this->camera.up), glm::vec3(this->camera.look));
    this->right = glm::cross(glm::vec3(this->camera.look), glm::vec3(this->camera.up));
}

void Scene::updateCameraPos(glm::vec4 direction) {
    this->camera.pos += direction;
}

glm::vec4 Scene::getTranslation(Qt::Key keyDown) {
    switch (keyDown) {
    case Qt::Key::Key_W:
        return this->camera.look;
        break;
    case Qt::Key::Key_S:
        return -this->camera.look;
        break;
    case Qt::Key::Key_A:
        return glm::vec4(this->left, 0.f);
        break;
    case Qt::Key::Key_D:
        return glm::vec4(this->right, 0.f);
        break;
    case Qt::Key::Key_Space:
        return glm::vec4(0.f, 1.0f, 0.f, 0.f);
        break;
    case Qt::Key::Key_Control:
        return glm::vec4(0.f, -1.0f, 0.f, 0.f);
        break;
    default:
        std::cerr << "Key invalid!" << std::endl;
        return glm::vec4(0.f,0.f,0.f,0.f);
        break;
    }
}
*/

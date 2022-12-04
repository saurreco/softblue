#include "scene.h"
#include "utils/errorthrowing.h"

void Scene::drawModel(Model model) {
    glUseProgram(shader);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // UNIFORMS -----------------------------------------------------------
    /* bind uniforms */
//    GLuint mvpLoc = glGetUniformLocation(shader, "mvp");
//    ErrorThrowing::uniformNotFound(mvpLoc, "mvp");
//    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

    // binding matrices
    GLuint viewMatrixLoc = glGetUniformLocation(shader, "viewMatrix");
    ErrorThrowing::uniformNotFound(viewMatrixLoc, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &viewMatrix[0][0]);

    GLuint projMatrixLoc = glGetUniformLocation(shader, "projMatrix");
    ErrorThrowing::uniformNotFound(projMatrixLoc, "projMatrix");
    glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &projMatrix[0][0]);

    GLuint modelMatrixLoc = glGetUniformLocation(shader, "modelMatrix");
    ErrorThrowing::uniformNotFound(modelMatrixLoc, "modelMatrix");
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model.ctm[0][0]); // defined in initModel

    // normalTransformMatrix is CURRENTLY equal to modelMatrix
    // TODO: change this!
    GLuint normalTransformMatrixLoc = glGetUniformLocation(shader, "normalTransformMatrix");
    ErrorThrowing::uniformNotFound(normalTransformMatrixLoc, "normalTransformMatrix");
    glUniformMatrix4fv(normalTransformMatrixLoc, 1, GL_FALSE, &model.ctm[0][0]); // defined in initModel

    // light stuff ------------------------------------------------
//    GLuint lightPosLoc = glGetUniformLocation(shader, "lightPos");
//    ErrorThrowing::uniformNotFound(lightPosLoc, "lightPos");
//    glUniform4fv(lightPosLoc, 1, &this->testPointLight.pos[0]);

    GLuint lightDirLoc = glGetUniformLocation(shader, "lightDir");
    ErrorThrowing::uniformNotFound(lightDirLoc, "lightDir");
    glUniform3fv(lightDirLoc, 1, &this->testPointLight.dir[0]);

    GLuint lightColorLoc = glGetUniformLocation(shader, "lightColor");
    ErrorThrowing::uniformNotFound(lightColorLoc, "lightColor");
    glUniform4fv(lightColorLoc, 1, &this->testPointLight.color[0]);

    GLuint camPosLoc = glGetUniformLocation(shader, "camPos");
    ErrorThrowing::uniformNotFound(camPosLoc, "camPos");
    // TODOS: REPLACE WITH ACTUAL CAM POS
    glm::vec3 camPos(0,0,-3);
    glUniform3fv(camPosLoc, 1, &camPos[0]);

    GLuint shininessLoc = glGetUniformLocation(shader, "shininess");
    ErrorThrowing::uniformNotFound(shininessLoc, "shininess");
    glUniform1f(shininessLoc, this->testModel.shininess);
    // light stuff ------------------------------------------------

    // ka kd ks ------------------------------------------------
    GLuint m_kaLoc = glGetUniformLocation(shader, "m_ka");
    ErrorThrowing::uniformNotFound(m_kaLoc, "m_ka");
    glUniform1f(m_kaLoc, this->ka);

    GLuint m_kdLoc = glGetUniformLocation(shader, "m_kd");
    ErrorThrowing::uniformNotFound(m_kdLoc, "m_kd");
    glUniform1f(m_kdLoc, this->kd);

    GLuint m_ksLoc = glGetUniformLocation(shader, "m_ks");
    ErrorThrowing::uniformNotFound(m_ksLoc, "m_ks");
    glUniform1f(m_ksLoc, this->ks);
    // ka kd ks ------------------------------------------------

    // object colors ----------------------------------------------
    GLuint objectAmbientLoc = glGetUniformLocation(shader, "objectAmbient");
    ErrorThrowing::uniformNotFound(objectAmbientLoc, "objectAmbient");
    glUniform4fv(objectAmbientLoc, 1, &this->testModel.ambient[0]);

    GLuint objectDiffuseLoc = glGetUniformLocation(shader, "objectDiffuse");
    ErrorThrowing::uniformNotFound(objectDiffuseLoc, "objectDiffuse");
    glUniform4fv(objectDiffuseLoc, 1, &this->testModel.diffuse[0]);

    GLuint objectSpecularLoc = glGetUniformLocation(shader, "objectSpecular");
    ErrorThrowing::uniformNotFound(objectSpecularLoc, "objectSpecular");
    glUniform4fv(objectSpecularLoc, 1, &this->testModel.specular[0]);
    // object colors ----------------------------------------------

    glBindVertexArray(model.geometry->vao);
    glDrawElements(GL_TRIANGLES, model.geometry->numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Scene::initModel() {
    this->testModel.ambient = glm::vec4(0.f, 1.f, 0.f, 1.f);
    this->testModel.diffuse = glm::vec4(0.f, 1.f, 0.f, 1.f);
    this->testModel.specular = glm::vec4(1.f, 1.f, 1.f, 1.f);
    this->testModel.shininess = 20.f;
    this->testModel.ctm = glm::mat4(1.f);
    this->testModel.geometry = new SphereMesh();
}

void Scene::addLight(glm::vec4 color, glm::vec3 pos, glm::vec3 dir) {
    this->testPointLight.color = color;
    this->testPointLight.pos = pos;
    this->testPointLight.dir = dir;
}

void Scene::matricesInit(int w, int h) {
    this->screenWidth = w;
    this->screenHeight = h;
    this->projMatrix = glm::perspective(45.0f, (this->screenWidth / (float)this->screenHeight), 0.1f, 100.0f);
//    this->viewMatrix = glm::lookAt(glm::vec3(this->camera.pos), glm::vec3(this->camera.pos + this->camera.look), glm::vec3(this->camera.up));
    this->initializeView();
}

/**
 * @brief Scene::sceneInit initializes a TEST SCENE
 * @param w
 * @param h
 */
void Scene::sceneInit(int w, int h) {
    this->cameraInit(w, h);
    this->matricesInit(w, h);
    this->initModel();
    this->addLight(glm::vec4(1.f), glm::vec4(3, 4, 5, 1), glm::vec4(0.f, -1.f, 0.f, 0.f));
}

// CAMERA FUNCTIONS ----------------------------
void Scene::cameraInit(int w, int h) {
    this->camera.up = glm::vec4(0, 1, 0, 0);
    this->camera.pos = glm::vec4(0, 0, -3, 0);
    this->camera.look = glm::vec4(0, 0, 3, 0);
}

/**
 * @brief Scene::calcRotationMatrix
 * @param axis is a normalized vector!
 * @param theta is the rotation angle
 */
glm::mat3 Scene::calcRotationMatrix(glm::vec3 axis, float theta) {
    float cos = glm::cos(theta);
    float sin = glm::sin(theta);
    float x = axis[0];
    float y = axis[1];
    float z = axis[2];
    // initializing in row major
    glm::mat3 rotationMatrix = glm::mat3(
                cos + x*x*(1-cos), x*y*(1-cos) - z*sin, x*z*(1-cos) + y*sin,
                x*y*(1-cos) + z*sin, cos + y*y*(1-cos), y*z*(1-cos) - x*sin,
                x*z*(1-cos) - y*sin, y*z*(1-cos) + x*sin, cos + z*z*(1-cos)
                );
    return rotationMatrix;
}

/**
 * @brief Scene::updateUpnLook update up * look vectors OF CAMERADATA
 * @param rotationMatrix
 */
void Scene::updateUpnLook(glm::mat3 rotationMatrix) {
    this->camera.look = glm::vec4(rotationMatrix * this->camera.look, 0.f);
    this->camera.up = glm::vec4(rotationMatrix * this->camera.up, 0.f);
}

/**
 * @brief Scene::initializeView initializes view-related stuff
 * called in matricesInit()
 */
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

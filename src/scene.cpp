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
    GLuint lightPosLoc = glGetUniformLocation(shader, "lightPos");
    ErrorThrowing::uniformNotFound(lightPosLoc, "lightPos");
    glUniform4fv(lightPosLoc, 1, &this->testPointLight.pos[0]);

    GLuint camPosLoc = glGetUniformLocation(shader, "camPos");
    ErrorThrowing::uniformNotFound(camPosLoc, "camPos");
    // TODOS: REPLACE WITH ACTUAL CAM POS
    glm::vec4 camPos(3,3,3,1);
    glUniform4fv(lightPosLoc, 1, &camPos[0]);

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

    glBindVertexArray(model.geometry->vao);
    glDrawElements(GL_TRIANGLE_STRIP, model.geometry->numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Scene::initModel() {
    this->testModel.ambient = glm::vec4(0.f);
    this->testModel.diffuse = glm::vec4(0.f);
    this->testModel.specular = glm::vec4(0.f);
    this->testModel.shininess = 1.f;
    this->testModel.ctm = glm::mat4(1.f);
    this->testModel.geometry = new SphereMesh();
}

void Scene::addLight(glm::vec4 color, glm::vec4 pos, glm::vec4 dir) {
    this->testPointLight.color = color;
    this->testPointLight.pos = pos;
    this->testPointLight.dir = dir;
}

void Scene::matricesInit(int w, int h) {
    this->screenWidth = w;
    this->screenHeight = h;
    this->projMatrix = glm::perspective(45.0f, (this->screenWidth / (float)this->screenHeight), 0.1f, 100.0f);
    this->viewMatrix = glm::lookAt(glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}
void Scene::sceneInit(int w, int h) {
    this->matricesInit(w, h);
    this->initModel();
    this->addLight(glm::vec4(1.f), glm::vec4(0, 2, 0, 1), glm::vec4(0.f));
}

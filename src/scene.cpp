#include "scene.h"

void Scene::drawModel(Model model) {
    glUseProgram(shader);
    /* bind uniforms */
    glBindVertexArray(model.geometry->vao);
    glDrawElements(GL_TRIANGLE_STRIP, model.geometry->numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Scene::initModel() {
    testModel.ambient = glm::vec4(0.f);
    testModel.diffuse = glm::vec4(0.f);
    testModel.specular = glm::vec4(0.f);
    testModel.shininess = 0.f;
    testModel.ctm = glm::mat4(1.f);
    testModel.geometry = new SphereMesh();
}

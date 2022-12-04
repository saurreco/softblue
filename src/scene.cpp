#include "scene.h"
#include <glm/gtx/string_cast.hpp>

void Scene::drawScene(Shader* shader, Camera* camera) {
    for (Model model : models) {
        drawModel(shader, camera, model);
    }
}

void Scene::drawModel(Shader* shader, Camera* camera, Model model) {
    shader->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // UNIFORMS -----------------------------------------------------------

    /* viewproj matrix */
    shader->addUniformMat4("viewProj", camera->viewProj);

    /* lightDir */
    shader->addUniformVec3("lightDir", light.dir);
    /* lightColor */
    shader->addUniformVec4("lightColor", light.color);
    /* camPos */
    shader->addUniformVec3("camPos", camera->pos);

    /* ambient */
    shader->addUniformVec4("ambient", model.ambient);
    /* diffuse */
    shader->addUniformVec4("diffuse", model.diffuse);
    /* specular */
    shader->addUniformVec4("specular", model.specular);

    glBindVertexArray(model.geometry->vao);
    glDrawElements(GL_TRIANGLES, model.geometry->numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shader->unbind();
}

void Scene::addModel(Mesh* mesh, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular) {
    Model model = Model{mesh, ambient, diffuse, specular};
    models.push_back(model);
}

void Scene::setLight(glm::vec4 color, glm::vec3 dir) {
    light.color = color;
    light.dir = dir;
}

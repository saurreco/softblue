#include "scene.h"
#include <glm/gtx/string_cast.hpp>

void Scene::drawScene(Shader* shader, Camera* camera) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (Model model : models) {
        drawModel(shader, camera, model);
    }
    // CUBEMAP ---------------------------------------------------------
    glDepthMask(GL_FALSE);
    shader->bind(ShaderType::CUBEMAP_SHADER);
    shader->addUniformMat4(ShaderType::CUBEMAP_SHADER, "viewProj", camera->viewProj); /* viewproj matrix */
    shader->addUniformInt(ShaderType::CUBEMAP_SHADER, "cubemap_texture", 0);
    this->cubemap->drawCubeMap();
    shader->unbind();
    glDepthMask(GL_TRUE);
    // CUBEMAP ---------------------------------------------------------
}

void Scene::drawModel(Shader* shader, Camera* camera, Model model) {
    shader->bind(ShaderType::MAIN_SHADER);

    // UNIFORMS -----------------------------------------------------------

    /* viewproj matrix */
    shader->addUniformMat4(ShaderType::MAIN_SHADER, "viewProj", camera->viewProj);

    /* lightDir */
    shader->addUniformVec3(ShaderType::MAIN_SHADER, "lightDir", light.dir);
    /* lightColor */
    shader->addUniformVec4(ShaderType::MAIN_SHADER, "lightColor", light.color);
    /* camPos */
    shader->addUniformVec3(ShaderType::MAIN_SHADER, "camPos", camera->pos);

    /* ambient */
    shader->addUniformVec4(ShaderType::MAIN_SHADER, "ambient", model.ambient);
    /* diffuse */
    shader->addUniformVec4(ShaderType::MAIN_SHADER, "diffuse", model.diffuse);
    /* specular */
    shader->addUniformVec4(ShaderType::MAIN_SHADER, "specular", model.specular);

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

void Scene::setupCubemap() {
    this->cubemap->initCubeMap(); // initialization
    const char* front = "/Users/os/Documents/CS-Classes/cs1230/projects/softblue/skybox/back.jpg";
    const char* back = "/Users/os/Documents/CS-Classes/cs1230/projects/softblue/skybox/front.jpg";
    const char* left = "/Users/os/Documents/CS-Classes/cs1230/projects/softblue/skybox/left.jpg";
    const char* right = "/Users/os/Documents/CS-Classes/cs1230/projects/softblue/skybox/right.jpg";
    const char* top = "/Users/os/Documents/CS-Classes/cs1230/projects/softblue/skybox/top.jpg";
    const char* bottom = "/Users/os/Documents/CS-Classes/cs1230/projects/softblue/skybox/bottom.jpg";
    this->cubemap->fillCubeMap(front, back, top, bottom, left, right);
}

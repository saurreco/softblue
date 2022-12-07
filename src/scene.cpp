#include "scene.h"
#include <glm/gtx/string_cast.hpp>

void Scene::drawScene(Shader* shader, Camera* camera) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemap->cubemap_texture);
    for (Model model : models) {
        drawModel(shader, camera, model);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindVertexArray(0);

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
    /* cubemap texture */
    shader->addUniformInt(ShaderType::MAIN_SHADER, "cubemap_texture", 0);

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
    const char* front = "/ifs/CS/replicated/home/tdougan1/csci-1230/softblue/skybox/back.jpg";
    const char* back = "/ifs/CS/replicated/home/tdougan1/csci-1230/softblue/skybox/front.jpg";
    const char* left = "/ifs/CS/replicated/home/tdougan1/csci-1230/softblue/skybox/left.jpg";
    const char* right = "/ifs/CS/replicated/home/tdougan1/csci-1230/softblue/skybox/right.jpg";
    const char* top = "/ifs/CS/replicated/home/tdougan1/csci-1230/softblue/skybox/top.jpg";
    const char* bottom = "/ifs/CS/replicated/home/tdougan1/csci-1230/softblue/skybox/bottom.jpg";
    this->cubemap->fillCubeMap(front, back, top, bottom, left, right);
}

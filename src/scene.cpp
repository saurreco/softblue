#include "scene.h"
#include <glm/gtx/string_cast.hpp>
#include "debug.h"

void printViewport()
{
    int vp[4] = {-1,-1,-1,-1};
    glGetIntegerv(GL_VIEWPORT, vp);
    std::cout << "viewport = "
              << vp[0] << ", "
              << vp[1] << ", "
              << vp[2] << ", "
              << vp[3] << std::endl;

}

void Scene::drawScene(Shader* shader, Camera* camera) {

//    printViewport();
    // dynamic env map
    this->cubemap->bindDynamic();
    glViewport(0, 0, this->cubemap->cubemapSideLength, this->cubemap->cubemapSideLength);
    Debug::glErrorCheck();

//    printViewport();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemap->cubemap_texture);
    for (Model model : models) {
        drawModel(shader, camera, model);
        break; // DRAW ONLY THE FIRST ONE!
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindVertexArray(0);
    Debug::glErrorCheck();

    // CUBEMAP ---------------------------------------------------------
    glDepthMask(GL_FALSE);
    shader->bind(ShaderType::CUBEMAP_SHADER);
    glm::mat4 viewTemp = glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,10,0), glm::vec3(1,0,0)); // is it 1,0,0?
    glm::mat4 projTemp = glm::perspective(90, 1, 1, 100);
    glm::mat4 viewProjTemp = projTemp * viewTemp;
    // camera->viewProj
    shader->addUniformMat4(ShaderType::CUBEMAP_SHADER, "viewProj", viewProjTemp); /* viewproj matrix */
    shader->addUniformInt(ShaderType::CUBEMAP_SHADER, "cubemap_texture", 0);
    this->cubemap->drawCubeMap();
    shader->unbind();
    glDepthMask(GL_TRUE);
    // CUBEMAP ---------------------------------------------------------
    Debug::glErrorCheck();

    this->cubemap->unbindDynamic();
    glViewport(0, 0, screenWidth, screenHeight);
//    printViewport();
    Debug::glErrorCheck();

//    // starting to paint the texture
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_FALSE);
    shader->bind(ShaderType::DYNAMIC_SHADER);
    shader->addUniformMat4(ShaderType::DYNAMIC_SHADER, "viewProj", camera->viewProj); /* viewproj matrix */
    shader->addUniformInt(ShaderType::DYNAMIC_SHADER, "cubemap_texture_side", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindVertexArray(this->cubemap->cubemapTop_vao);
    this->cubemap->bindCubesideTex();
    glDrawArrays(GL_TRIANGLES, 0, 6); // only TOP
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDepthMask(GL_TRUE);
    shader->unbind();
}

void Scene::drawModel(Shader* shader, Camera* camera, Model model) {
    shader->bind(ShaderType::MAIN_SHADER);

    // UNIFORMS -----------------------------------------------------------
    /* cubemap texture */
    shader->addUniformInt(ShaderType::MAIN_SHADER, "cubemap_texture", 0);

    // FOR NOW render from bottom to top!
    glm::mat4 viewTemp = glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,10,0), glm::vec3(1,0,0)); // is it 1,0,0?
    glm::mat4 projTemp = glm::perspective(90, 1, 1, 100);
    glm::mat4 viewProjTemp = projTemp * viewTemp;

    /* viewproj matrix */
    shader->addUniformMat4(ShaderType::MAIN_SHADER, "viewProj", viewProjTemp); // camera->viewProj

    /* lightDir */
    shader->addUniformVec3(ShaderType::MAIN_SHADER, "lightDir", light.dir);
    /* lightColor */
    shader->addUniformVec4(ShaderType::MAIN_SHADER, "lightColor", light.color);
    glm::vec3 tempCampos = glm::vec3(0,0,0);
    /* camPos */
    shader->addUniformVec3(ShaderType::MAIN_SHADER, "camPos", tempCampos); // camera->pos

    /* ambient */
    shader->addUniformVec4(ShaderType::MAIN_SHADER, "ambient", model.ambient);
    /* diffuse */
    shader->addUniformVec4(ShaderType::MAIN_SHADER, "diffuse", model.diffuse);
    /* specular */
    shader->addUniformVec4(ShaderType::MAIN_SHADER, "specular", model.specular);

    /* env mapping */
    shader->addUniformInt(ShaderType::MAIN_SHADER, "isEnvMapped", model.isEnvMapped);

    glBindVertexArray(model.geometry->vao);
    glDrawElements(GL_TRIANGLES, model.geometry->numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shader->unbind();
}

void Scene::addModel(Mesh* mesh, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, bool isEnvMapped) {
    Model model = Model{mesh, ambient, diffuse, specular, isEnvMapped};
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
    Debug::glErrorCheck();
    this->cubemap->fillCubeMap(front, back, top, bottom, left, right);
    Debug::glErrorCheck();
}

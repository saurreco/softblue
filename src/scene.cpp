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

void Scene::drawFboSide(Shader* shader, Camera* camera) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    for (Model model : models) {
        drawModel(shader, camera, model);
        break; // DRAW ONLY THE FIRST ONE!
    }
    glBindVertexArray(0);

    // CUBEMAP ---------------------------------------------------------
    glDepthMask(GL_FALSE);
    shader->bind(ShaderType::CUBEMAP_SHADER);
    // camera->viewProj
    shader->addUniformMat4(ShaderType::CUBEMAP_SHADER, "viewProj", camera->viewProj); /* viewproj matrix */
    shader->addUniformInt(ShaderType::CUBEMAP_SHADER, "cubemap_texture", 0);
    this->cubemap->drawCubeMap();
    shader->unbind();
    glDepthMask(GL_TRUE);
    // CUBEMAP ---------------------------------------------------------
}

void Scene::drawScene(Shader* shader, Camera* camera) {
    // dynamic env map
    this->cubemap->bindDynamic();
    glViewport(0, 0, this->cubemap->cubemapSideLength, this->cubemap->cubemapSideLength);

    // draw each fbo side here
    for (int i = 0; i < 6; i++) {
        switch (i) {
        case 0:
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, this->cubemap->dynamicCubemap->fbo_tex_cube, 0);
            this->drawFboSide(shader, rightCamera);
            break;
        case 1:
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, this->cubemap->dynamicCubemap->fbo_tex_cube, 0);
            this->drawFboSide(shader, leftCamera);
            break;
        case 2:
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, this->cubemap->dynamicCubemap->fbo_tex_cube, 0);
            this->drawFboSide(shader, topCamera);
            break;
        case 3:
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, this->cubemap->dynamicCubemap->fbo_tex_cube, 0);
            this->drawFboSide(shader, bottomCamera);
            break;
        case 4:
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, this->cubemap->dynamicCubemap->fbo_tex_cube, 0);
            this->drawFboSide(shader, backCamera);
            break;
        case 5:
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, this->cubemap->dynamicCubemap->fbo_tex_cube, 0);
            this->drawFboSide(shader, frontCamera);
            break;
        default:
            std::cerr << "side does not exist" << std::endl;
            break;
        }
    }

    GLenum fboState = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboState != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "incomplete" << std::endl;
        switch (fboState) {
        case GL_FRAMEBUFFER_UNDEFINED:
            std::cout << "undefined" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cout << "incomplete attachment" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cout << "incomplete missing attachment" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            std::cout << "incomplete draw buffer" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            std::cout << "incomplete read buffer" << std::endl;
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cout << "unsupported" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            std::cout << "incomplete multisample" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            std::cout << "incomplete layer targets" << std::endl;
            break;
        default:
            std::cout << "not sure" << std::endl;
            break;
        }
    }

    this->cubemap->unbindDynamic();
    glViewport(0, 0, screenWidth, screenHeight);

    // starting to paint the texture
    glClearColor(0.5f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_FALSE);
    shader->bind(CUBEMAP_SHADER);
    shader->addUniformMat4(ShaderType::CUBEMAP_SHADER, "viewProj", camera->viewProj); /* viewproj matrix */
    shader->addUniformInt(ShaderType::CUBEMAP_SHADER, "cubemap_texture", 1); // binding to texture 1
    glActiveTexture(GL_TEXTURE1);
    this->cubemap->bindCubesideTex();
    glBindVertexArray(this->cubemap->cubemapVao); // draw the fbo texture on cubemap vao
    glDrawArrays(GL_TRIANGLES, 0, 36); // whole cube
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glDepthMask(GL_TRUE);
    shader->unbind();

    // render the model
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    this->cubemap->bindCubesideTex(); // binding fbo_tex_cube, the texture of the cube
    int count = 0;
    for (Model model : models) {
        drawModel(shader, camera, model);
        if (count == 0) {
            continue;
        } // DRAW ONLY THE SECOND ONE!
        count++;
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindVertexArray(0);

//     CUBEMAP ---------------------------------------------------------
    glDepthMask(GL_FALSE);
    shader->bind(ShaderType::CUBEMAP_SHADER);
    // camera->viewProj
    shader->addUniformMat4(ShaderType::CUBEMAP_SHADER, "viewProj", camera->viewProj); /* viewproj matrix */
//    shader->addUniformInt(ShaderType::CUBEMAP_SHADER, "cubemap_texture", 0);
  //  this->cubemap->drawCubeMap();
    shader->unbind();
    glDepthMask(GL_TRUE);
    // CUBEMAP ---------------------------------------------------------
}

void Scene::drawModel(Shader* shader, Camera* camera, Model model) {
    shader->bind(ShaderType::MAIN_SHADER);

    // UNIFORMS -----------------------------------------------------------
    /* cubemap texture */
  //  shader->addUniformInt(ShaderType::MAIN_SHADER, "cubemap_texture", 0);

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

void Scene::initializeSideCameras() {
    // CURRENTLY SETTING BOTH WIDTH & HEIGHT TO 1.f TO MAINTAIN 1:1 RATIO!
    // init top camera:
    this->topCamera->init(1.f, 1.f, glm::vec3(0,0,0), glm::vec3(0,10,0), glm::vec3(0,0,1), 90.f);
    // init bottom camera:
    this->bottomCamera->init(1.f, 1.f,  glm::vec3(0,0,0), glm::vec3(0,-10,0), glm::vec3(0,0,-1), 90.f);
    // init left camera:
    this->leftCamera->init(1.f, 1.f, glm::vec3(0,0,0), glm::vec3(-10,0,0), glm::vec3(0,-1,0), 90.f);
    // init right camera:
    this->rightCamera->init(1.f, 1.f,  glm::vec3(0,0,0), glm::vec3(10,0,0), glm::vec3(0,-1,0), 90.f);
    // init front camera:
    this->frontCamera->init(1.f, 1.f, glm::vec3(0,0,0), glm::vec3(0,0,-10), glm::vec3(0,-1,0), 90.f); // -1, 90
    // init back camera:
    this->backCamera->init(1.f, 1.f, glm::vec3(0,0,0), glm::vec3(0,0,10), glm::vec3(0,-1,0), 90.f);
}

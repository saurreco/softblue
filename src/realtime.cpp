#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/gtx/string_cast.hpp>

#include "settings.h"
#include "utils/sceneparser.h"
#include "utils/shaderloader.h"
#include "meshgen.h"
#include "debug.h"

// ================== Project 5: Lights, Camera

// ================== FBO =========================================================

void Realtime::makeFBO(){

    // Task 19: Generate and bind an empty texture, set its min/mag filter interpolation, then unbind
    Debug::glErrorCheck();
    glGenTextures(1, &fbo_texture);
    Debug::glErrorCheck();
    glBindTexture(GL_TEXTURE_2D, fbo_texture);
    Debug::glErrorCheck();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbo_width, fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    Debug::glErrorCheck();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    Debug::glErrorCheck();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Debug::glErrorCheck();
    glBindTexture(GL_TEXTURE_2D, 0);
    // Task 20: Generate and bind a renderbuffer of the right size, set its format, then unbind
    Debug::glErrorCheck();
    glGenRenderbuffers(1, &fbo_renderbuffer);
    Debug::glErrorCheck();
    glBindRenderbuffer(GL_RENDERBUFFER, fbo_renderbuffer);
    Debug::glErrorCheck();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fbo_width, fbo_height);
    Debug::glErrorCheck();
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // Task 18: Generate and bind an FBO
    Debug::glErrorCheck();
    glGenFramebuffers(1, &fbo);
    Debug::glErrorCheck();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // Task 21: Add our texture as a color attachment, and our renderbuffer as a depth+stencil attachment, to our FBO
    Debug::glErrorCheck();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
    Debug::glErrorCheck();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo_renderbuffer);
    // Task 22: Unbind the FBO
    Debug::glErrorCheck();

    glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
}

void Realtime::deleteFBO(){
    glDeleteTextures(1, &fbo_texture);
    glDeleteRenderbuffers(1, &fbo_renderbuffer);
    glDeleteFramebuffers(1, &fbo);
}

void Realtime::initScreenGeometry() {


}

void Realtime::paintTexture(int filter){

    glUseProgram(texture_shader);
     // Task 32: Set your bool uniform on whether or not to filter the texture drawn
     glUniform1i(glGetUniformLocation(texture_shader, "isBussin"), 1);

     // Task 10: Bind "texture" to slot 0
     glBindVertexArray(screen_vao);
     glBindTexture(GL_TEXTURE_2D, fbo_texture);


     glDrawArrays(GL_TRIANGLES, 0, 6);
     glBindVertexArray(0);
     glUseProgram(0);

}
// ================== REALTIME =========================================================

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;

    // If you must use this function, do not edit anything above this
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here

    this->doneCurrent();
}

glm::mat4 perspective(float fovy, float aspect, float near, float far) {

   float viewPlaneH = 2 * tan(fovy / 2.0);
   float viewPlaneW = viewPlaneH * aspect;
   float fovx = 2 * atan((viewPlaneW / 2.0f));

   float c = -near / (float)far;

   glm::mat4 scaling = glm::mat4(1 / (float)(far * tan(fovx / 2.0f)), 0, 0, 0,
                           0, 1 / (float)(far * tan(fovy / 2.0f)), 0, 0,
                           0, 0, 1 / (float)far, 0,
                           0, 0, 0, 1);

   glm::mat4 unhinge = glm::mat4(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1 / (float)(1 + c), -c / (float)(1 + c),
                            0, 0, -1, 0);

   glm::mat4 remapping = glm::mat4(1, 0, 0, 0,
                                   0, 1, 0, 0,
                                   0, 0, -2, -1,
                                   0, 0, 0, 1);

   return  glm::transpose(remapping) * glm::transpose(unhinge) * glm::transpose(scaling);
}

void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    // Initializing GL.
    // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GL: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION) << std::endl;

    // Allows OpenGL to draw objects appropriately on top of one another
    glEnable(GL_DEPTH_TEST);
    // Tells OpenGL to only draw the front face
    glEnable(GL_CULL_FACE);
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);
    fbo_width = size().width() * m_devicePixelRatio;
    fbo_height = size().height() * m_devicePixelRatio;
     defaultFBO = 2;
    // Students: anything requiring OpenGL calls when the program starts should be done here
     // Set some default values for the OpenGL context
     glClearColor(0, 0, 0, 1);
     glEnable(GL_CULL_FACE);
     glEnable(GL_DEPTH_TEST);

    // Shader setup (DO NOT EDIT)
    m_shader = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert", ":/resources/shaders/default.frag");
    texture_shader = ShaderLoader::createShaderProgram(":/resources/shaders/texture.vert", ":/resources/shaders/texture.frag");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    // ================== Vertex Buffer Objects

        m_sphereData = Meshgen::genSphere(settings.shapeParameter2, settings.shapeParameter1);
        m_cubeData = Meshgen::genCube(settings.shapeParameter1, settings.shapeParameter2);
        m_cylinderData = Meshgen::genCylinder(settings.shapeParameter1, settings.shapeParameter2);
        m_coneData = Meshgen::genCone(settings.shapeParameter1, settings.shapeParameter2);

    // ================== Vertex Array Objects

    // Task 11: Generate a VAO here and store it in m_vao
    glGenBuffers(1, &m_sphereVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, m_sphereData.size() * sizeof(GLfloat), m_sphereData.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_sphereVAO);
    glBindVertexArray(m_sphereVAO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * 4, 0); // xyz
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * 4, reinterpret_cast<void*>(3 * 4)); // nx ny nz
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * 4, reinterpret_cast<void*>(6 * 4));


    glGenBuffers(1, &m_cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, m_cubeData.size() * sizeof(GLfloat), m_cubeData.data(), GL_STATIC_DRAW);


    glGenVertexArrays(1, &m_cubeVAO);
    glBindVertexArray(m_cubeVAO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * 4, 0); // xyz
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * 4, reinterpret_cast<void*>(3 * 4)); // nx ny nz
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * 4, reinterpret_cast<void*>(6 * 4));

    glGenBuffers(1, &m_cylinderVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_cylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, m_cylinderData.size() * sizeof(GLfloat), m_cylinderData.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_cylinderVAO);
    glBindVertexArray(m_cylinderVAO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * 4, 0); // xyz
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * 4, reinterpret_cast<void*>(3 * 4)); // nx ny nz
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * 4, reinterpret_cast<void*>(6 * 4));

    glGenBuffers(1, &m_coneVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_coneVBO);
    glBufferData(GL_ARRAY_BUFFER, m_coneData.size() * sizeof(GLfloat), m_coneData.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_coneVAO);
    glBindVertexArray(m_coneVAO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * 4, 0); // xyz
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * 4, reinterpret_cast<void*>(3 * 4)); // nx ny nz
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * 4, reinterpret_cast<void*>(6 * 4));

    glActiveTexture(GL_TEXTURE0);
    Debug::glErrorCheck();
    glBindTexture(GL_TEXTURE_2D, 0);
    Debug::glErrorCheck();
    glUseProgram(texture_shader);
    Debug::glErrorCheck();
    glUniform1i(glGetUniformLocation(texture_shader, "tex"), 0);
    Debug::glErrorCheck();

    std::vector<GLfloat> fullscreen_quad_data =
    { //     POSITIONS    //
        -1.0f,  1.0f, 0.0f,
         0.0f,  1.0f,
        -1.0f, -1.0f, 0.0f,
         0.0f,  0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  0.0f,
         1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,
        -1.0f,  1.0f, 0.0f,
         0.0f,  1.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  0.0f
    };
    // Generate and bind a VBO and a VAO for a fullscreen quad
    glGenBuffers(1, &screen_vbo);
    Debug::glErrorCheck();
    glBindBuffer(GL_ARRAY_BUFFER, screen_vbo);
    Debug::glErrorCheck();
    glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size()*sizeof(GLfloat), fullscreen_quad_data.data(), GL_STATIC_DRAW);
    Debug::glErrorCheck();
    glGenVertexArrays(1, &screen_vao);
    Debug::glErrorCheck();
    glBindVertexArray(screen_vao);

    // Task 14: modify the code below to add a second attribute to the vertex attribute array
    Debug::glErrorCheck();
    glEnableVertexAttribArray(0);
    Debug::glErrorCheck();
    glEnableVertexAttribArray(1);
    Debug::glErrorCheck();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    Debug::glErrorCheck();
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void*>(3 * 4));
    Debug::glErrorCheck();

    // Unbind the fullscreen quad's VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    makeFBO();

    // ================== Returning to Default State
}

void Realtime::drawGeometry() {
    glUseProgram(m_shader);

    for (Mesh mesh : m_meshes) {

        int dataSize = 0;
        int vao = 0;
        int vbo;
        switch(mesh.type) {
            case 1:
                vbo = m_sphereVBO;
                vao = m_sphereVAO;
                dataSize = m_sphereData.size() / 8;
                break;
            case 2:
                vbo = m_cubeVBO;
                vao = m_cubeVAO;
                dataSize = m_cubeData.size() / 8;
                break;
            case 3:
                vbo = m_cylinderVBO;
                vao = m_cylinderVAO;
                dataSize = m_cylinderData.size() / 8;
                break;
            case 4:
                vbo = m_coneVBO;
                vao = m_coneVAO;
                dataSize = m_coneData.size() / 8;
                break;
        }

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glUniformMatrix4fv(glGetUniformLocation(m_shader, "model"), 1, GL_FALSE, &mesh.model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "normalTransform"), 1, GL_FALSE, &mesh.normalTransform[0][0]);

        glUniform4fv(glGetUniformLocation(m_shader, "material.ambient"), 1, &mesh.material.ambient[0]);
        glUniform4fv(glGetUniformLocation(m_shader, "material.diffuse"), 1, &mesh.material.diffuse[0]);
        glUniform4fv(glGetUniformLocation(m_shader, "material.specular"), 1, &mesh.material.specular[0]);
        glUniform1f(glGetUniformLocation(m_shader, "material.shininess"), mesh.material.shininess);

        for (int i = 0; i < m_numLights; i++) {

            std::string angle = "lights[" + std::to_string(i) + "].angle";
            std::string type = "lights[" + std::to_string(i) + "].type";
            std::string attenuation = "lights[" + std::to_string(i) + "].attenuation";
            std::string color = "lights[" + std::to_string(i) + "].color";
            std::string dir = "lights[" + std::to_string(i) + "].dir";
            std::string penumbra = "lights[" + std::to_string(i) + "].penumbra";
            std::string pos = "lights[" + std::to_string(i) + "].pos";

            glUniform1i(glGetUniformLocation(m_shader, type.c_str()), m_lights[i].type);
            glUniform1f(glGetUniformLocation(m_shader, angle.c_str()), m_lights[i].angle);
            glUniform3fv(glGetUniformLocation(m_shader, attenuation.c_str()), 1, &m_lights[i].attenuation[0]);
            glUniform4fv(glGetUniformLocation(m_shader, color.c_str()), 1, &m_lights[i].color[0]);
            glUniform3fv(glGetUniformLocation(m_shader, dir.c_str()), 1, &m_lights[i].dir[0]);
            glUniform3fv(glGetUniformLocation(m_shader, pos.c_str()), 1, &m_lights[i].pos[0]);
            glUniform1f(glGetUniformLocation(m_shader, penumbra.c_str()), m_lights[i].penumbra);
        }

        glUniform1f(glGetUniformLocation(m_shader, "ka"), m_ka);
        glUniform1f(glGetUniformLocation(m_shader, "kd"), m_kd);
        glUniform1f(glGetUniformLocation(m_shader, "ks"), m_ks);

        glUniformMatrix4fv(glGetUniformLocation(m_shader, "view"), 1, GL_FALSE, &m_view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "proj"), 1, GL_FALSE, &m_proj[0][0]);

        glUniform3fv(glGetUniformLocation(m_shader, "camPos"), 1, &m_camPos[0]);
        glUniform1f(glGetUniformLocation(m_shader, "numLights"), m_numLights);

        glDrawArrays(GL_TRIANGLES, 0, dataSize);
    }
    glBindVertexArray(0);
    glUseProgram(0);
}

void Realtime::paintGL() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, m_width, m_height);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
      //  std::cout << "yoooo" << std::endl;
    } else {
     //   std::cout << "noooo" << std::endl;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawGeometry();



    glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
    glViewport(0, 0, m_width, m_height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    paintTexture(1);

}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);
    m_width = size().width();
    m_height = size().height();
    if (scene) {
        float fovy = m_renderData.cameraData.heightAngle;
        float aspect = m_width / (float) m_height;
        m_proj = perspective(fovy, aspect, settings.nearPlane, settings.farPlane);
    }
   fbo_height = h;
   fbo_width = w;
   deleteFBO();
   makeFBO();


    // Students: anything requiring OpenGL calls when the program starts should be done here
}

void Realtime::sceneChanged() {

    scene = 1;

    /**
     *
     * WHAT CHANGES:
     *
     * vert:
     * - model matrix
     * - normal transform
     * - view matrix
     * - projection matrix
     *
     * frag:
     * - lights
     * - cam pos
     * - material
     * - global ka, kd, ks
     *
     */

    m_meshes.clear();
    SceneParser::parse(settings.sceneFilePath, m_renderData);
    for (RenderShapeData shape : m_renderData.shapes) {
        Mesh mesh;
        mesh.material.ambient = shape.primitive.material.cAmbient;

        mesh.material.diffuse = shape.primitive.material.cDiffuse;
        mesh.material.specular = shape.primitive.material.cSpecular;
        mesh.material.shininess = shape.primitive.material.shininess;
        mesh.model = shape.ctm;
        mesh.normalTransform = glm::mat4(glm::inverse(glm::transpose(glm::mat3(shape.ctm))));

        switch (shape.primitive.type) {
            case PrimitiveType::PRIMITIVE_SPHERE:
                mesh.type = 1;
                break;
            case PrimitiveType::PRIMITIVE_CUBE:
                mesh.type = 2;
                break;
            case PrimitiveType::PRIMITIVE_CYLINDER:
                mesh.type = 3;
                break;
            case PrimitiveType::PRIMITIVE_CONE:
                mesh.type = 4;
                break;
        }

        m_meshes.push_back(mesh);
    }

    /* view */
    m_pos = m_renderData.cameraData.pos;
    m_look = m_renderData.cameraData.look;
    m_up = m_renderData.cameraData.up;
    m_view = glm::lookAt(m_pos, m_look, m_up);

    /* proj */
    float fovy = m_renderData.cameraData.heightAngle;
    float aspect = m_width / (float) m_height;
    m_proj = perspective(fovy, aspect, settings.nearPlane, settings.farPlane);

    /* camPos */
    m_camPos = glm::vec4(m_pos, 1);

    /* global ka, kd, ks */
    m_ka = m_renderData.globalData.ka;
    m_kd = m_renderData.globalData.kd;
    m_ks = m_renderData.globalData.ks;

    /* lights */
    m_numLights = fmin(8, m_renderData.lights.size());
    for (int i = 0; i < m_numLights; i++) {
        m_lights[i].angle = m_renderData.lights[i].angle;
        m_lights[i].attenuation = m_renderData.lights[i].function;
        m_lights[i].color = m_renderData.lights[i].color;
        m_lights[i].dir = m_renderData.lights[i].dir;
        m_lights[i].penumbra = m_renderData.lights[i].penumbra;
        m_lights[i].pos = m_renderData.lights[i].pos;

        switch(m_renderData.lights[i].type) {
            case LightType::LIGHT_DIRECTIONAL:
                m_lights[i].type = 0;
                break;
            case LightType::LIGHT_POINT:
                m_lights[i].type = 1;
                break;
            case LightType::LIGHT_SPOT:
                m_lights[i].type = 2;
                break;
        }
    }

    /* gl uniform bindings */

    /* lights */


    /* fill m_meshes */
    update(); // asks for a PaintGL() call to occur
}

void Realtime::settingsChanged() {
    m_sphereData = Meshgen::genSphere(settings.shapeParameter1, settings.shapeParameter2);
    m_cubeData = Meshgen::genCube(settings.shapeParameter1, settings.shapeParameter2);
    m_cylinderData = Meshgen::genCylinder(settings.shapeParameter1, settings.shapeParameter2);
    m_coneData = Meshgen::genCone(settings.shapeParameter1, settings.shapeParameter2);


    if (m_sphereVBO != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, m_sphereVBO);
        glBufferData(GL_ARRAY_BUFFER, m_sphereData.size() * sizeof(GLfloat), m_sphereData.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if (m_cubeVBO != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, m_cubeData.size() * sizeof(GLfloat), m_cubeData.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if (m_cylinderVBO != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, m_cylinderVBO);
        glBufferData(GL_ARRAY_BUFFER, m_cylinderData.size() * sizeof(GLfloat), m_cylinderData.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if (m_coneVBO != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, m_coneVBO);
        glBufferData(GL_ARRAY_BUFFER, m_coneData.size() * sizeof(GLfloat), m_coneData.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if (scene) {
        float fovy = m_renderData.cameraData.heightAngle;
        float aspect = m_width / (float) m_height;
        m_proj = perspective(fovy, aspect, settings.nearPlane, settings.farPlane);

    }

    update();
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = false;
    }
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);
        float dx = -3.14159 * 0.2 * (deltaX / (float)m_width);
        float dy = 3.14159 * 0.2 * (deltaY / (float)m_height);
        m_look = m_look * glm::mat3(glm::rotate(dx, glm::vec3(0, 1, 0)));
        m_look = m_look * glm::mat3(glm::rotate(dy, glm::cross(m_pos - m_look, m_up)));
        glm::vec3 pt = m_pos - m_up;
        pt = pt * glm::mat3(glm::rotate(dy, glm::cross(m_pos - m_look, m_up)));
       // m_up = m_pos - pt;
        m_view = glm::lookAt(m_pos, m_look, m_up);

std::cout << glm::to_string(m_up) << std::endl;
        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();
    glm::vec3 moveVector(0, 0, 0);
    glm::vec3 forward = normalize(m_pos - m_look);
    glm::vec3 left = normalize(cross(forward, m_up));
    m_up = normalize(m_up);
    if (m_keyMap.at(Qt::Key::Key_W))
        moveVector += -1.0f * forward;
    if (m_keyMap.at(Qt::Key::Key_S))
        moveVector += 1.0f * forward;
    if (m_keyMap.at(Qt::Key::Key_A))
        moveVector += 1.0f * left;
    if (m_keyMap.at(Qt::Key::Key_D))
        moveVector += -1.0f * left;
    if (m_keyMap.at(Qt::Key::Key_Space))
        moveVector += 1.0f * m_up;
    if (m_keyMap.at(Qt::Key::Key_Control))
        moveVector += -1.0f * m_up;
    if (moveVector != glm::vec3(0, 0, 0))
        moveVector = 5.0f * normalize(moveVector) * deltaTime;
    m_pos = m_pos + moveVector;
    m_camPos = glm::vec4(m_pos, 1);
    m_look = m_pos - forward;
    m_view = glm::lookAt(m_pos, m_look, m_up);

    update();
}

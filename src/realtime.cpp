#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/gtx/string_cast.hpp>

#include "utils/sceneparser.h"
#include "utils/shaderloader.h"
#include "debug.h"

// ================== Project 5: Lights, Camera

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
    delete(this->scene);
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
    // Students: anything requiring OpenGL calls when the program starts should be done here

    // Shader setup (DO NOT EDIT)
    this->scene->shader = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert", ":/resources/shaders/default.frag");
    this->scene->initModel(); // TESTING
}

void Realtime::paintGL() {
    // paint here
    this->scene->drawModel(this->scene->testModel); // TESTING
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);
    m_width = size().width();
    m_height = size().height();
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
//    if (m_mouseDown) {
//        int posX = event->position().x();
//        int posY = event->position().y();
//        int deltaX = posX - m_prev_mouse_pos.x;
//        int deltaY = posY - m_prev_mouse_pos.y;
//        m_prev_mouse_pos = glm::vec2(posX, posY);
//        float dx = -3.14159 * 0.2 * (deltaX / (float)m_width);
//        float dy = 3.14159 * 0.2 * (deltaY / (float)m_height);
//        m_look = m_look * glm::mat3(glm::rotate(dx, glm::vec3(0, 1, 0)));
//        m_look = m_look * glm::mat3(glm::rotate(dy, glm::cross(m_pos - m_look, m_up)));
//        glm::vec3 pt = m_pos - m_up;
//        pt = pt * glm::mat3(glm::rotate(dy, glm::cross(m_pos - m_look, m_up)));
//       // m_up = m_pos - pt;
//        m_view = glm::lookAt(m_pos, m_look, m_up);

//std::cout << glm::to_string(m_up) << std::endl;
//        update(); // asks for a PaintGL() call to occur
//    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    update();
}

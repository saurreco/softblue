#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/gtx/string_cast.hpp>

#include "utils/shaderloader.h"
#include "debug.h"
#include "cubemesh.h"

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
    m_width = size().width() * m_devicePixelRatio;
    m_height = size().height() * m_devicePixelRatio;
    this->scene = new Scene();
    this->camera = new Camera();
    this->shader = new Shader();

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

    /* setup scene */
    shader->set(":/resources/shaders/default.vert", ":/resources/shaders/default.frag");
    scene->addModel(new SphereMesh(),
                    glm::vec4(0, 0.5, 0, 1),
                    glm::vec4(0, 1, 0, 1),
                    glm::vec4(0, 1, 0, 1));
    scene->setLight(glm::vec4(1, 1, 1, 1), glm::vec3(-1, -1, -1));
    camera->init(m_width, m_height, glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

}

void Realtime::paintGL() {
    // paint here
    this->scene->drawScene(shader, camera);
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);
    m_width = size().width() * m_devicePixelRatio;
    m_height = size().height() * m_devicePixelRatio;
    // update w & h in Scene, construct matrices elsewhere in scene & not in constructor
    camera->setFrustum(w, h);
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    Qt::Key keyPressed = Qt::Key(event->key());
    // if invalid key then ignore
    if (keyPressed != Qt::Key::Key_W &&
        keyPressed != Qt::Key::Key_A &&
        keyPressed != Qt::Key::Key_S &&
        keyPressed != Qt::Key::Key_D &&
        keyPressed != Qt::Key::Key_Space &&
        keyPressed != Qt::Key::Key_Control) {
        return;
    }
    m_keyMap[keyPressed] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    Qt::Key keyPressed = Qt::Key(event->key());
    // if invalid key then ignore
    if (keyPressed != Qt::Key::Key_W &&
        keyPressed != Qt::Key::Key_A &&
        keyPressed != Qt::Key::Key_S &&
        keyPressed != Qt::Key::Key_D &&
        keyPressed != Qt::Key::Key_Space &&
        keyPressed != Qt::Key::Key_Control) {
        return;
    }
    m_keyMap[keyPressed] = false;
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
        /*
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        // Use deltaX and deltaY here to rotate
        // speed? decided by theta! how about pi/4 every 1000 pixels moved
        // rotate camera about axis (0,1,0)
        // rotate camera about axis that is the right vector
        // speedX = thetaX = deltaX/screen width * pi / 4
        // speedY = thetaY = deltaX/screen width * pi / 4
        // combine 2 rotation matrices
        // update view matrix & related data

        float thetaX = (float)deltaX / 1000.f * M_PI / 4;
        glm::mat3 rotationX = glm::rotate(glm::vec3(0.f,1.f,0.f), thetaX);
        float thetaY = (float)deltaY / 1000.f * M_PI / 4;
        glm::mat3 rotationY = glm::rotate(this->scene->right, thetaY);
        glm::mat3 combinedRotation = rotationX * rotationY;
        this->scene->updateUpnLook(combinedRotation);
        this->scene->initializeView();
            */
        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();
/*
    // find length to move here
    // if delta time is 0.2 then we move 1 unit! (1/5 of the standard number of units per second)
    float noOfMovingWorldUnit = deltaTime * 5; // deltaTime is counted in seconds
    // find which key is down
    glm::vec4 returnedMovement = this->findKeyMovement();
    if (returnedMovement == glm::vec4(0.f)) {
        return; // no movement at all
    }
    // then get the normalized direction
    glm::vec4 normalizedReturnedMovement = glm::normalize(returnedMovement);
    // then move in the length we specified earlier
    // change camera's pos
    this->scene->updateCameraPos(noOfMovingWorldUnit * normalizedReturnedMovement);
    // update view matrix
    this->scene->initializeView();
*/
    update(); // asks for a PaintGL() call to occur
}

// MOVEMENT HELPERS ----------------------------------------------
glm::vec4 Realtime::findKeyMovement() {
    glm::vec4 totalDirection(0.f);
    for (auto it = this->m_keyMap.begin(); it != this->m_keyMap.end(); ++it) {
        if (it->second) { // if it's true => key is down
            // totalDirection += this->scene->getTranslation(it->first);
        }
    }
    return totalDirection;
}
// MOVEMENT HELPERS ----------------------------------------------

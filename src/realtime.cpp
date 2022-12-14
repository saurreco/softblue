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
#include "physics.h"
#include "tetmesh.h"

// ================== Project 5: Lights, Camera

// ================== REALTIME =========================================================

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_prev_mouse_pos = glm::vec2(size().width() / 2, size().height() / 2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W] = false;
    m_keyMap[Qt::Key_A] = false;
    m_keyMap[Qt::Key_S] = false;
    m_keyMap[Qt::Key_D] = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space] = false;

    // If you must use this function, do not edit anything above this
}

void Realtime::finish()
{
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here

    this->doneCurrent();
    delete (this->scene);
}

void Realtime::initializeGL()
{
    m_devicePixelRatio = this->devicePixelRatio();
    m_width = size().width() * m_devicePixelRatio;
    m_height = size().height() * m_devicePixelRatio;
    this->scene = new Scene();
    this->camera = new Camera();
    this->shader = new Shader();
    this->physics = new Physics();

    m_timer = startTimer(1000 / 60);
    m_elapsedTimer.start();

    // Initializing GL.
    // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
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
    shader->set(ShaderType::MAIN_SHADER, ":/shaders/default.vert", ":/shaders/default.frag");
    shader->set(ShaderType::CUBEMAP_SHADER, ":/shaders/cubemap.vert", ":/shaders/cubemap.frag");
    // scene->addModel(new SphereMesh(glm::translate(glm::mat4(1), glm::vec3(0, 4, 0))),
    //                 glm::vec4(0, 0.5, 0, 1),
    //                 glm::vec4(0, 1, 0, 1),
    //                 glm::vec4(0, 1, 0, 1),
    //                 false);
    // scene->addModel(new SphereMesh(glm::translate(glm::mat4(1), glm::vec3(0, 0, 0))),
    //                 glm::vec4(0, 0.5, 0, 1),
    //                 glm::vec4(0, 1, 0, 1),
    //                 glm::vec4(0, 1, 0, 1),
    //                 true);
    //    scene->addModel(new SphereMesh(),
    // shader->set(":/shaders/default.vert", ":/shaders/default.frag");
    // scene->addModel(new CubeMesh(glm::translate(glm::mat4(1), glm::vec3(0, 6, 0))),
    // shader->set(":/shaders/default.vert", ":/shaders/default.frag");
    scene->addModel(new SphereMesh(2, glm::translate(glm::scale(glm::mat4(1), glm::vec3(1.f)), glm::vec3(3, 12, 0))), // x = -3 y = 12
                    // glm::vec3(0.4, 0.4, 0.4)
                    glm::vec4(0.f, 0.f, 0.5f, 1),
                    glm::vec4(1, 1, 1, 1), glm::vec4(1, 1, 1, 1), true, 1, glm::vec3(3, 12, 0),
                    10000.0f, 10000.0f);
    scene->addModel(new SphereMesh(2, glm::translate(glm::mat4(1), glm::vec3(2, 3, 0))),
                    glm::vec4(0.1, 0.2, 0.5, 1),
                    glm::vec4(0.2, 0.5, 0.9, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    1,
                    glm::vec3(2, 3, 0),
                    900.0f, 500.0f);
    scene->addModel(new SphereMesh(2, glm::translate(glm::mat4(1), glm::vec3(-3, 5, 0))),
                    glm::vec4(0.1, 0.2, 0.5, 1),
                    glm::vec4(0.2, 0.8, 0.9, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    1,
                    glm::vec3(-3, 5, 0),
                    1000.0f, 200.0f);
    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(-0.4f, glm::vec3(0, 0, 1)), glm::vec3(5, 1, 3)), glm::vec3(-0.5, 0, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(-0.5, 0, 0), 0, 0);

    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(0.5f, glm::vec3(0, 0, 1)), glm::vec3(6, 1, 3)), glm::vec3(0.5, -5, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(0.5, -5, 0), 0, 0);
    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(0.0f, glm::vec3(0, 0, 1)), glm::vec3(1.5, 1, 2)), glm::vec3(0, -8.5, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(-0.5, -10, 0), 0, 0);

    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(-0.28f, glm::vec3(0, 0, 1)), glm::vec3(2.3, 1, 2)), glm::vec3(1.6, -8.0, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(-0.5, -10, 0), 0, 0);


    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(0.28f, glm::vec3(0, 0, 1)), glm::vec3(2.3, 1, 2)), glm::vec3(-1.6, -8.0, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(-0.5, -10, 0), 0, 0);

    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(0.55f, glm::vec3(0, 0, 1)), glm::vec3(2.3, 1, 2)), glm::vec3(-3.0, -6.6, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(-0.5, -10, 0), 0, 0);

    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(-0.55f, glm::vec3(0, 0, 1)), glm::vec3(2.3, 1, 2)), glm::vec3(3.0, -6.6, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(-0.5, -10, 0), 0, 0);

    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(0.3f, glm::vec3(0, 0, 1)), glm::vec3(10, 1, 3)), glm::vec3(0.2, -14.8, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(-0.5, -10, 0), 0, 0);


    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(-0.3f, glm::vec3(0, 0, 1)), glm::vec3(10, 1, 3)), glm::vec3(-0.2, -14.8, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(-0.5, -10, 0), 0, 0);

    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(-0.0f, glm::vec3(0, 0, 1)), glm::vec3(1, 11, 3)), glm::vec3(-1.7, -2, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(-0.5, -10, 0), 0, 0);

    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(-0.0f, glm::vec3(0, 0, 1)), glm::vec3(1, 11, 3)), glm::vec3(1.7, -2, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(-0.5, -10, 0), 0, 0);

    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(-0.0f, glm::vec3(0, 0, 1)), glm::vec3(5, 1, 3)), glm::vec3(0.60, -32, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(-0.5, -10, 0), 0, 0);

    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(-0.0f, glm::vec3(0, 0, 1)), glm::vec3(5, 1, 3)), glm::vec3(-0.60, -36, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(-0.5, -10, 0), 0, 0);

    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(-0.0f, glm::vec3(0, 0, 1)), glm::vec3(5, 1, 3)), glm::vec3(0.60, -40, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(-0.5, -10, 0), 0, 0);

    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(-0.0f, glm::vec3(0, 0, 1)), glm::vec3(5, 1, 3)), glm::vec3(-0.60, -44, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(-0.5, -10, 0), 0, 0);

    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(-0.0f, glm::vec3(0, 0, 1)), glm::vec3(5, 1, 3)), glm::vec3(0.60, -48, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(-0.5, -10, 0), 0, 0);

    scene->addModel(new CubeMesh(glm::translate(glm::scale(glm::rotate(-0.0f, glm::vec3(0, 0, 1)), glm::vec3(5, 1, 3)), glm::vec3(-0.60, -62, 0))),
                    glm::vec4(0.5, 0.5, 0.5, 1),
                    glm::vec4(1, 1, 1, 1),
                    glm::vec4(1, 1, 1, 1),
                    false,
                    0,
                    glm::vec3(-0.5, -10, 0), 0, 0);

    scene->setLight(glm::vec4(1, 1, 1, 1), glm::vec3(-1, -1, -1));
    scene->setupCubemap();
    this->scene->screenWidth = m_width;
    this->scene->screenHeight = m_height;
    scene->initializeSideCameras();
//    camera->init(m_width, m_height, glm::vec3(0, 0, 10), glm::vec3(0, 0, -3), glm::vec3(0, 1, 0), 45.f);

    // scene->setLight(glm::vec4(1, 1, 1, 1), glm::vec3(-1, -1, -1));
    // scene->setupCubemap();
    camera->init(m_width, m_height, glm::vec3(0, 5, 20), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), 45.f, 1.f);
    physics->init(scene);
}

void Realtime::paintGL()
{
    // paint here
    this->scene->drawScene(shader, camera);
}

void Realtime::resizeGL(int w, int h)
{
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);
    m_width = size().width() * m_devicePixelRatio;
    m_height = size().height() * m_devicePixelRatio;
    this->scene->screenWidth = m_width;
    this->scene->screenHeight = m_height;
    // update w & h in Scene, construct matrices elsewhere in scene & not in constructor
    camera->setFrustum(w, h, 45.f, 1.f);
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event)
{
    Qt::Key keyPressed = Qt::Key(event->key());
    // if invalid key then ignore
    if (keyPressed != Qt::Key::Key_W &&
        keyPressed != Qt::Key::Key_A &&
        keyPressed != Qt::Key::Key_S &&
        keyPressed != Qt::Key::Key_D &&
        keyPressed != Qt::Key::Key_Space &&
        keyPressed != Qt::Key::Key_Control)
    {
        return;
    }
    m_keyMap[keyPressed] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event)
{
    Qt::Key keyPressed = Qt::Key(event->key());
    // if invalid key then ignore
    if (keyPressed != Qt::Key::Key_W &&
        keyPressed != Qt::Key::Key_A &&
        keyPressed != Qt::Key::Key_S &&
        keyPressed != Qt::Key::Key_D &&
        keyPressed != Qt::Key::Key_Space &&
        keyPressed != Qt::Key::Key_Control)
    {
        return;
    }
    m_keyMap[keyPressed] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::LeftButton))
    {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event)
{
    if (!event->buttons().testFlag(Qt::LeftButton))
    {
        m_mouseDown = false;
    }
}

void Realtime::mouseMoveEvent(QMouseEvent *event)
{
    if (m_mouseDown)
    {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        // Use deltaX and deltaY here to rotate
        float thetaX = -(float)deltaX / 1000.f * M_PI / 4;
        glm::mat4 rotationX = glm::rotate(thetaX, glm::vec3(0.f, 1.f, 0.f));
        float thetaY = -(float)deltaY / 1000.f * M_PI / 4;
        glm::mat4 rotationY = glm::rotate(thetaY, this->camera->right);
        glm::mat3 combinedRotation = rotationX * rotationY;
        this->camera->updateUpnLook(combinedRotation);
        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::tmp(float dt)
{
    physics->update(dt / 4.0f);
    physics->update(dt / 4.0f);
    physics->update(dt / 4.0f);
    physics->update(dt / 4.0f);
}

void Realtime::timerEvent(QTimerEvent *event)
{
    // rectangular prism rotations
//    time += M_PI/32;

    // physics
    int elapsedms = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    tmp(0.001);
    tmp(0.001);
    tmp(0.001);
    tmp(0.001);
    tmp(0.001);
    tmp(0.001);
    tmp(0.001);
    tmp(0.001);
    tmp(0.001);
    physics->updateScene();
//    this->scene->models[9].geometry->translateX(time);

//    this->scene->updateReflectiveObjectPosition(this->physics->objectCenter);
//    std::cout << glm::to_string(physics->objectCenter) << std::endl;
    // reinitialize side cams
    this->scene->initializeSideCameras();

    // a    physics->update(deltaTime);
    camera->panDown(0.02f);
    m_elapsedTimer.restart();
    /*
        // find length to move here
        // if delta time is 0.2 then we move 1 unit! (1/5 of the standard number of units per second)
        float noOfMovingWorldUnit = deltaTime * 5; // deltaTime is counted in seconds
        // find which key is down
        glm::vec3 returnedMovement = this->findKeyMovement();
        if (returnedMovement == glm::vec3(0.f)) {
            return; // no movement at all
        }
        // then get the normalized direction
        glm::vec3 normalizedReturnedMovement = glm::normalize(returnedMovement);
        // then move in the length we specified earlier
        // change camera's pos
        this->camera->updateCameraPos(noOfMovingWorldUnit * normalizedReturnedMovement);
        // update view matrix
    //*/
    update(); // asks for a PaintGL() call to occur
}

// MOVEMENT HELPERS ----------------------------------------------
glm::vec3 Realtime::findKeyMovement()
{
    glm::vec3 totalDirection(0.f);
    for (auto it = this->m_keyMap.begin(); it != this->m_keyMap.end(); ++it)
    {
        if (it->second)
        { // if it's true => key is down
            totalDirection += this->camera->getTranslation(it->first);
        }
    }
    return totalDirection;
}
// MOVEMENT HELPERS ----------------------------------------------

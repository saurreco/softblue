#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>
#include "utils/sceneparser.h"

struct Light {
    int type;
    glm::vec4 color;
    glm::vec3 attenuation;
    glm::vec3 pos;
    glm::vec3 dir;
    float penumbra;
    float angle;
};

struct Material {
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float shininess;
};

struct Mesh {
    int type;
    int size;
    glm::mat4 model;
    glm::mat4 normalTransform;
    Material material;
};


class Realtime : public QOpenGLWidget
{
public:
    Realtime(QWidget *parent = nullptr);
    void finish();                                      // Called on program exit
    void sceneChanged();
    void settingsChanged();

public slots:
    void tick(QTimerEvent* event);                      // Called once per tick of m_timer

protected:
    void initializeGL() override;                       // Called once at the start of the program
    void paintGL() override;                            // Called whenever the OpenGL context changes or by an update() request
    void resizeGL(int width, int height) override;      // Called when window size changes
    void swapVBO(std::vector<float> data);

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    glm::mat4 lookAt(glm::vec3 pos, glm::vec3 look, glm::vec3 up);


    // Tick Related Variables
    int m_timer;                                        // Stores timer which attempts to run ~60 times per second
    QElapsedTimer m_elapsedTimer;                       // Stores timer which keeps track of actual time between frames

    // Input Related Variables
    bool m_mouseDown = false;                           // Stores state of left mouse button
    glm::vec2 m_prev_mouse_pos;                         // Stores mouse position
    std::unordered_map<Qt::Key, bool> m_keyMap;         // Stores whether keys are pressed or not

    // Device Correction Variables
    int m_devicePixelRatio;

    std::vector<Mesh> m_meshes;
    RenderData m_renderData;
    int scene = 0;

    /* camera descriptors */
    glm::vec3 m_look;
    glm::vec3 m_up;
    glm::vec3 m_pos;

    glm::mat4 m_view;
    glm::mat4 m_proj;
    glm::vec4 m_camPos;
    float m_ka;
    float m_kd;
    float m_ks;

    int m_width;
    int m_height;

    struct Light m_lights[8];
    int m_numLights;

    GLuint m_vao;
    GLuint m_shader;

    GLuint m_sphereVAO;
    GLuint m_cubeVAO;
    GLuint m_cylinderVAO;
    GLuint m_coneVAO;

    GLuint m_sphereVBO = 0;
    GLuint m_cubeVBO = 0;
    GLuint m_cylinderVBO = 0;
    GLuint m_coneVBO = 0;

    void drawGeometry();

    // FBO
    void makeFBO();
    void deleteFBO();
    void paintTexture(int f);
    void initScreenGeometry();
    GLuint fbo_width;
    GLuint fbo_height;
    GLuint fbo;
    GLuint fbo_texture;
    GLuint fbo_renderbuffer;

    GLuint screen_vbo;
    GLuint screen_vao;
    GLuint texture_shader;

    GLuint defaultFBO;


    std::vector<GLfloat> m_sphereData;
    std::vector<GLfloat> m_cubeData;
    std::vector<GLfloat> m_cylinderData;
    std::vector<GLfloat> m_coneData;
};

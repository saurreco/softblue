#ifndef MESHGEN_H
#define MESHGEN_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>

namespace Meshgen {

std::vector<GLfloat> genSphere(int p1, int p2);
std::vector<GLfloat> genCube(int p1, int p2);
std::vector<GLfloat> genCone(int p1, int p2);
std::vector<GLfloat> genCylinder(int p1, int p2);


}
#endif // MESHGEN_H



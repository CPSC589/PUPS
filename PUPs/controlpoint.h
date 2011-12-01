#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

#include <GL/glu.h>
#include <QString>
#include <QGLWidget>
#include <QtGui>
#include <Point.h>

class ControlPoint: public Point
{
public:
    ControlPoint();
    void changeWeight(GLfloat newWeight);
    void changeLeftRadius(GLfloat newRadius);
    void changeRightRadius(GLfloat newRadius);
    GLfloat getWeight();
    GLfloat getLeftRadius();
    GLfloat getRightRadius();
private:
    GLfloat weight;
    GLfloat leftRadius;
    GLfloat rightRadius;
};

#endif // CONTROLPOINT_H

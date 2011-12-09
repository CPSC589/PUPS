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
    ControlPoint(Point convertPoint);
    void changeWeight(double newWeight);
    GLfloat getWeight();
private:
    GLfloat weight;
};

#endif // CONTROLPOINT_H

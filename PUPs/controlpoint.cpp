#include "controlpoint.h"

ControlPoint::ControlPoint()
{
    weight = 1;
}

ControlPoint::ControlPoint(Point convertPoint){
    x = convertPoint.x;
    y = convertPoint.y;
    z = convertPoint.z;
}

void ControlPoint::changeWeight(double newWeight){
    weight = newWeight;
}

GLfloat ControlPoint::getWeight(){
    return weight;
}

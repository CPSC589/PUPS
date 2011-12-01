#include "controlpoint.h"

ControlPoint::ControlPoint()
{
    weight = 1;
}

void ControlPoint::changeWeight(GLfloat newWeight){
    weight = newWeight;
}

void ControlPoint::changeLeftRadius(GLfloat newRadius){
    leftRadius = newRadius;
}

void ControlPoint::changeRightRadius(GLfloat newRadius){
    rightRadius = newRadius;
}

GLfloat ControlPoint::getWeight(){
    return weight;
}

GLfloat ControlPoint::getLeftRadius(){
    return leftRadius;
}

GLfloat ControlPoint::getRightRadius(){
    return rightRadius;
}

#ifndef STATE_H
#define STATE_H

#include <Point.h>
#include <vector>
#include "controlpoint.h"


class State
{
public:
    State();
    void saveControlPoints(vector<ControlPoint> ctrlPoints);
    vector<ControlPoint> loadControlPoints();
private:
    vector<ControlPoint> ControlPoints;
};

#endif // STATE_H

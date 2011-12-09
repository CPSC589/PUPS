#ifndef STATE_H
#define STATE_H

#include <Point.h>
#include <vector>
#include "controlpoint.h"
#include "pup.h"

class State
{
public:
    State(Pup pupCurve);
    Pup getPupCurve();

private:
    Pup PupCurve;
};

#endif // STATE_H

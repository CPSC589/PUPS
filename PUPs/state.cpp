#include "state.h"
#include "controlpoint.h"


//=================================================================
// State class keeps track of the particular state that a system is
// in.  Any time a change is made to the system (i.e. changing the
// position of a control point, adding a control point, removing
// a control point, changing a weight, etc) a new state will be
// created.  This is used to allow for an undo-redo feature, along
// with a fade feature.
//=================================================================
State::State(Pup pupCurve)
{
    PupCurve = pupCurve;
}

Pup State::getPupCurve(){
    return PupCurve;
}

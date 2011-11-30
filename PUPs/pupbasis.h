#ifndef PUPBASIS_H
#define PUPBASIS_H

#include <vector>
#include "nurbs.h"

//NURBS CURVE
class PupBasis
{
public:

    PupBasis(Nurbs _basis_function = Nurbs(), double _radius_left = 1.5, double _radius_right = 1.5, bool _is_relative_radius = true){
        basis_function = _basis_function;
        if (_is_relative_radius){
            relative_radius_left = _radius_left;
            relative_radius_right = _radius_right;
            actual_radius_left = actual_radius_right = 0;
        } else {
            actual_radius_left = _radius_left;
            actual_radius_right = _radius_right;
            relative_radius_left = relative_radius_right = 0;
        }
        is_relative_radius = _is_relative_radius;
        center_offset = 0;
    }

    Nurbs basis_function;

    //means the radius should be multiplied by the knot spacing to get the true radius
    bool is_relative_radius;

    //left and right radii of support
    double relative_radius_left;
    double relative_radius_right;
    double actual_radius_left;
    double actual_radius_right;

    //offset of the center of the basis function from its default uniform position
    double center_offset;

};


#endif // PUPBASIS_H

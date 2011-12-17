#ifndef PUP_H
#define PUP_H

#include <vector>
#include "point.h"
#include "nurbs.h"

struct BasisInfluence {
    int basis_index;
    double basis_scalar;
    BasisInfluence(int _basis_index = -1, double _basis_scalar = 0){
        basis_index = _basis_index;
        basis_scalar = _basis_scalar;
    }
};

class Pup
{
public:
    Pup();
    Pup(Nurbs _default_basis, double _default_weight = 1.0, double _u_increment = 0.001, vector<Point> _control_points = vector<Point>(), vector<Nurbs> _basis_functions = vector<Nurbs>(), vector<double> _weights = vector<double>());
    ~Pup();

    //defaults for when creating new control points
        Nurbs default_basis;
        double default_weight;
    //operations on defaults
        void setDefaultBasis(Nurbs _basis);
        void setDefaultWeight(double _weight);

    //control point data
        vector<Point> control_points;
    //generic control point operations
        void addControlPoint(Point); //uses default weight and basis
        void removeControlPoint(int _index);
        void modifyControlPointPosition(Point _new_position, int _index);
    //currently selected (or selectable) control point operations
        void modifyCurrentControlPointPosition(Point _new_position);
    //selection data & operations
        int selected_point_index;
        int selectable_point_index;
        void selectControlPoint(int _index);
        void updateSelectableControlPointIndex(float _selection_radius, Point _mouse_position);

    //weight data
        vector<double> weights;
    //weight operations
        void modifyCurrentControlPointWeight(double _weight); //currently selected control point
        void modifyControlPointWeight(double _weight,int _index);

     //basis data
        vector<Nurbs> basis_functions;
        //each outer vector represents a "u value" based on the "u increment" of the pups curve/parameter space, each inner vector contains the points from each basis at that "u value", paired with the basis' index
        //by organising it this way, the user can move a control point without us having to recalculate basis influence & normalising those influences
        vector< vector<BasisInfluence> > basis_influences;
        vector<double> normalising_coefficients;
        bool closed;

    //basis operations

        //basis control point & selection operations
        void addBasisControlPoint(Point _cp, double weight = 1.0);
        void addBasisControlPoints(double weight = 1.0);
        void removeBasisControlPoint(int _index);
        void modifyBasisControlPointPosition(Point _new_position, int _index);
        void modifyCurrentBasisControlPointPosition(Point _new_position);
        void modifyBasisControlPointWeight(double _new_weight, int _index);
        void modifyCurrentBasisControlPointWeight(double _new_weight);
        int selected_basis_point_index;
        int selectable_basis_point_index;
        int selectable_basis_line_index;
        void selectBasisControlPoint(int _index);
        void updateSelectableBasisControlPointIndex(float _selection_radius, Point _mouse_position);
        void stretchBasisLeft(double _new_left);
        void stretchBasisRight(double _new_right);
        void updateBasisInfluences();

    //Curve detail
        double u_increment;
        void setIncrement(double _u_increment);

    //curve data (will be the same size as the outer vector of "normalised_influences"
        vector<Point> curve_points;

private:
    double bFunc(int i, double u);
    double cheatFunc(int i, double u);
    int last_i;
    int last_ci;
    //for when basis centers or radii are changed, or pup control points are added/removed
    void updateCentersAndRadii();
    //for when basis functions are changed, and if the above func is called

    //for when pup control points are moved
    void updateCurvePoints();
    void setupDefaultBasis();
};

#endif

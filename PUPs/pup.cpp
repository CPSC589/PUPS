#include "pup.h"
#include <math.h>

Pup::Pup()
{
    //defaults
    setupDefaultBasis();
    default_weight = 1.0;
    u_increment = 0.001;

    //empties
    control_points = vector<Point>();
    curve_points = vector<Point>();
    basis_functions = vector<Nurbs>();
    weights = vector<double>();
    basis_influences = vector< vector<BasisInfluence> >();
    normalising_coefficients = vector<double>();
    selected_point_index = -1;
    selectable_point_index = -1;
    selected_basis_point_index = -1;
    selectable_basis_point_index = -1;
    last_i = 0;
    last_ci = 0;
}

Pup::Pup(Nurbs _default_basis, double _default_weight, double _u_increment, vector<Point> _control_points, vector<Nurbs> _basis_functions, vector<double> _weights)
{
    //given
    default_basis = _default_basis;
    default_weight = _default_weight;
    u_increment = _u_increment;
    control_points = _control_points;
    basis_functions = _basis_functions;
    weights = _weights;

    //empty
    curve_points = vector<Point>();
    basis_influences = vector< vector<BasisInfluence> >();
    normalising_coefficients = vector<double>();
    selected_point_index = -1;
    selectable_point_index = -1;
    selected_basis_point_index = -1;
    selectable_basis_point_index = -1;
    last_i = 0;
    last_ci = 0;

    updateBasisInfluences();
}

Pup::~Pup(){

}

void Pup::setDefaultBasis(Nurbs _basis){
    default_basis = _basis;
}

void Pup::setDefaultWeight(double _weight){
    default_weight = _weight;
}

void Pup::addControlPoint(Point _cp)
{
    control_points.push_back(_cp);
    weights.push_back(default_weight);
    selected_point_index = control_points.size()-1;
    selected_basis_point_index = 0;

    if (basis_functions.size() == 0)
    {
        //resized the default basis to fit in the parameter space
        double stretch = 1.0/(default_basis.control_points[default_basis.control_points.size()-1].x - default_basis.control_points[0].x);

        if (stretch!=0)
        {
            for (int j = 0; j < default_basis.control_points.size(); j++){
                default_basis.control_points[j].x *= stretch;
                default_basis.control_points[j].x += (1-stretch);
            }
            default_basis.updateCurve();
        }
        basis_functions.push_back(default_basis);
    }
    else
    {
        double order = 3.5;
        double overlay = 0.0;
        //squish multiplier
        double squish2 = (1.0*basis_functions.size()+ order + overlay + 1)/(basis_functions.size() + order + 2);
        //squish multiplier for smoother curve (more overlap in basis functions)

        //shift variables
        double shift_left, shift_right, basis_size = 0;

        //default basis squish/shift
        shift_left = default_basis.control_points[0].x;
        basis_size = default_basis.control_points[default_basis.control_points.size()-1].x - default_basis.control_points[0].x;
        //right shift ensures the last control point is at u=1
        shift_right = 1-basis_size*squish2;
        for (int j = 0; j < default_basis.control_points.size(); j++)
        {
            default_basis.control_points[j].x -= shift_left;
            default_basis.control_points[j].x *= squish2;
            default_basis.control_points[j].x += shift_right;
        }
        default_basis.updateCurve();
        //add the new basis function
        basis_functions.push_back(default_basis);

        //for the other basis functions...
        for (int i = 0; i < basis_functions.size()-1; i++)
        {
            //left shift amount is distance of first control point from 0
            shift_left = basis_functions[i].control_points[0].x;
            //right shift is the left_shift - one half the difference between the old basis size and new basis size
            basis_size = basis_functions[i].control_points[basis_functions[i].control_points.size()-1].x - basis_functions[i].control_points[0].x;
            double temp_shift_right = shift_right * i/(basis_functions.size()-1);

            for (int j = 0; j < basis_functions[i].control_points.size(); j++)
            {
                //shift function to the left
                basis_functions[i].control_points[j].x -= shift_left;
                //squish it
                basis_functions[i].control_points[j].x *= squish2;
                //shift it back by the shift_left amount
                basis_functions[i].control_points[j].x += temp_shift_right;
            }
            basis_functions[i].updateCurve();
        }
    }
    updateBasisInfluences();
}
    //if (normalising_denominator == 0)

void Pup::removeControlPoint(int _index)
{
    control_points.erase(control_points.begin()+_index);
    basis_functions.erase(basis_functions.begin()+_index);
    weights.erase(weights.begin()+_index);

    if (selected_point_index > _index){
        selected_point_index--;
    } else if (selected_point_index == _index){
        selected_point_index = -1;
        selected_basis_point_index = -1;
    }
    selectable_point_index = -1;

    //stretch the last function to span the whole param space, and also the default basis
    if (basis_functions.size() == 1)
    {
        //resize the single basis to fit in the parameter space
        double stretch = 1.0/(basis_functions[0].control_points[basis_functions[0].control_points.size()-1].x - basis_functions[0].control_points[0].x);

        if (stretch!=0)
        {
            for (int j = 0; j < basis_functions[0].control_points.size(); j++){
                basis_functions[0].control_points[j].x *= stretch;
            }
            basis_functions[0].updateCurve();
        }

        //resize the default basis for when it is used later
        stretch = 1.0/(default_basis.control_points[default_basis.control_points.size()-1].x - default_basis.control_points[0].x);

        if (stretch!=0)
        {
            for (int j = 0; j < default_basis.control_points.size(); j++){
                default_basis.control_points[j].x *= stretch;
            }
            default_basis.updateCurve();
        }
    }
    else
    {
        //deletion order should be the same as when adding control points (see addcontrolpoint function)
        double order = 3.5;
        double overlay = 0.0;
        //squish multiplier
        double squish2 = (1.0*basis_functions.size() + order+2)/(basis_functions.size()+ 1+order + overlay );

        //shift variables
        double shift_left, shift_right, basis_size = 0;

        for (int i = 0; i < basis_functions.size(); i++)
        {
            //left shift amount is distance of first control point from 0
            shift_left = basis_functions[i].control_points[0].x;
            basis_size = basis_functions[i].control_points[basis_functions[i].control_points.size()-1].x - basis_functions[i].control_points[0].x;
            shift_right = (1-basis_size*squish2) * i/(basis_functions.size()-1);

            for (int j = 0; j < basis_functions[i].control_points.size(); j++)
            {
                //shift function to the left
                basis_functions[i].control_points[j].x -= shift_left;
                //squish it
                basis_functions[i].control_points[j].x *= squish2;
                //shift it back by the shift_left amount
                basis_functions[i].control_points[j].x += shift_right;
            }
            basis_functions[i].updateCurve();
        }

        //do the same for the default basis function
        shift_left = default_basis.control_points[0].x;
        basis_size = default_basis.control_points[default_basis.control_points.size()-1].x - default_basis.control_points[0].x;
        for (int j = 0; j < default_basis.control_points.size(); j++)
        {
            default_basis.control_points[j].x -= shift_left;
            default_basis.control_points[j].x *= squish2;
        }
        default_basis.updateCurve();

    }
    updateBasisInfluences();
}

void Pup::modifyControlPointPosition(Point _new_position, int _index){
    control_points[_index] = _new_position;
    updateCurvePoints();
}

void Pup::modifyCurrentControlPointPosition(Point _new_position){
    modifyControlPointPosition(_new_position,selected_point_index);
}

void Pup::modifyControlPointWeight(double _weight, int _index){
    weights[_index] = _weight;
    updateCurvePoints();
}

void Pup::modifyCurrentControlPointWeight(double _weight){
    modifyControlPointWeight(_weight,selected_point_index);
}

void Pup::selectControlPoint(int _index){
    selected_point_index = _index;
    selected_basis_point_index = 0;
}

void Pup::updateSelectableControlPointIndex(float _selection_radius, Point _mouse_position)
{  
    double current_dist;
    double closest_dist = _selection_radius;
    selectable_point_index = -1;

    //loop through all control points to see if any are within selection radius
    for (unsigned int i = 0; i < control_points.size(); i++){
        current_dist = (_mouse_position - control_points[i]).magnitude();
        if (current_dist < closest_dist){
            selectable_point_index = i;
            closest_dist = current_dist;
        }
    }
}

void Pup::addBasisControlPoint(Point _cp, double _weight)
{
    if (selected_point_index == -1){
        return;
    }

    ///////////////// PROBLEM WITH ADDING CPS /////////////////////////////////
    // Problem: u distance between all cps is equal, but x dist in parameter space is not
    // Solution 1: relocate old control points to reflect where they are affecting the basis (cps must be uniformly spaced)
    // Implementation for adding cps:
    //  1) get total size of new basis (rightmost - leftmost cps)
    //  2) divide amount by number of cps, loop through cps making their x value = first x value + its index * divided amount
    // Solution 2: get basis influences not from giving the NURBS curve u values, but instead by looping through all the curve points and interpolating the influence at specific x values
    // Implementation:
    //  1) Instead of using calling getSinglePoint(u) on NURBS curve, iterate through the curve points until current point < u and next point > u
    //  2) Get the y difference between those two points, the influence at the given u will be the first points y_value of first point + (y_difference)*(u-first_point_x)/(second_point_x-first_point_x)
    ///////////////////////////////////////////////////////////////////////////

    // UPDATE: SOLUTION 1 DOESNT WORK DUE TO STANDARD KNOT SEQUENCE OF NURBS
    // Using solution 2.

    //check if the control point is to the left or right of the basis function
    if (_cp.x < basis_functions[selected_point_index].control_points[0].x){

        //add the new left most control point
        basis_functions[selected_point_index].control_points.insert(basis_functions[selected_point_index].control_points.begin(),_cp);
        basis_functions[selected_point_index].weights.insert(basis_functions[selected_point_index].weights.begin(),_weight);
        //select it
        selected_basis_point_index = 0;

        /* SOLUTION 1 CODE (DOESNT QUITE WORK)
        //get new size of basis
        double new_size = basis_functions[selected_point_index].control_points[basis_functions[selected_point_index].control_points.size()-1].x - _cp.x;
        //get new x step for the control points
        double x_step = new_size/basis_functions[selected_point_index].control_points.size();
        //distribute the x values of the control points uniformly
        for (int i = 1; i < basis_functions[selected_point_index].control_points.size(); i++){
            basis_functions[selected_point_index].control_points[i].x = _cp.x + i*x_step;
        }*/

    } else if (_cp.x > basis_functions[selected_point_index].control_points[basis_functions[selected_point_index].control_points.size()-1].x){
        basis_functions[selected_point_index].control_points.push_back(_cp);
        basis_functions[selected_point_index].weights.push_back(_weight);
        selected_basis_point_index = basis_functions[selected_point_index].control_points.size()-1;
    } else {
        //place the control point between two others to avoid loops
        for (int i = 0; i < basis_functions[selected_point_index].control_points.size()-1; i++){
            if ((_cp.x > basis_functions[selected_point_index].control_points[i].x)&&(_cp.x < basis_functions[selected_point_index].control_points[i+1].x)){
                basis_functions[selected_point_index].control_points.insert(basis_functions[selected_point_index].control_points.begin()+i+1,_cp);
                basis_functions[selected_point_index].weights.insert(basis_functions[selected_point_index].weights.begin()+i+1,_weight);
                selected_basis_point_index = i;
                break;
            }
        }
    }
    //update nurbs curve
    basis_functions[selected_point_index].updateAll();
    //update this curve
    updateBasisInfluences();
}

void Pup::removeBasisControlPoint(int _index)
{
    if (selected_point_index == -1){
        return;
    }

    if (selected_basis_point_index > _index){
        selected_basis_point_index--;
    } else if (selected_basis_point_index == _index){
        selected_basis_point_index = -1;
    }
    selectable_basis_point_index = -1;

    //control point
    basis_functions[selected_point_index].control_points.erase( basis_functions[selected_point_index].control_points.begin() + _index);
    //weight
    basis_functions[selected_point_index].weights.erase( basis_functions[selected_point_index].weights.begin() + _index);

    //update basis curve
    basis_functions[selected_point_index].updateAll();
    //update this curve
    updateBasisInfluences();
}

void Pup::modifyBasisControlPointPosition(Point _new_position, int _index)
{
    if (selected_point_index == -1){
        return;
    }
    //control point
    basis_functions[selected_point_index].control_points[_index].y = _new_position.y;
    //update basis curve
    basis_functions[selected_point_index].updateCurve();
    //update this curve
    updateBasisInfluences();
}

void Pup::modifyCurrentBasisControlPointPosition(Point _new_position)
{
    modifyBasisControlPointPosition(_new_position,selected_basis_point_index);
}

void Pup::modifyBasisControlPointWeight(double _new_weight, int _index){
    if (selected_point_index == -1){
        return;
    }
    //weight
    basis_functions[selected_point_index].weights[_index] = _new_weight;
    //update basis curve
    basis_functions[selected_point_index].updateCurve();
    //update this curve
    updateBasisInfluences();
}

void Pup::modifyCurrentBasisControlPointWeight(double _new_weight){
    modifyBasisControlPointWeight(_new_weight,selected_basis_point_index);
}

void Pup::selectBasisControlPoint(int _index){
    selected_basis_point_index = _index;
}

void Pup::updateSelectableBasisControlPointIndex(float _selection_radius, Point _mouse_position)
{
    if (selected_point_index == -1){
        return;
    }

    double current_dist;
    double closest_dist = _selection_radius;
    selectable_basis_point_index = -1;

    //loop through all control points to see if any are within selection radius
    for (unsigned int i = 0; i < basis_functions[selected_point_index].control_points.size(); i++){
        current_dist = (_mouse_position - basis_functions[selected_point_index].control_points[i]).magnitude();
        if (current_dist < closest_dist){
            selectable_basis_point_index = i;
            closest_dist = current_dist;
        }
    }
}

void Pup::setIncrement(double _u_increment){
    u_increment = _u_increment;
    updateBasisInfluences();
}

void Pup::updateBasisInfluences()
{
    //this was a source of a heap error for a while
    for (int i = 0; i < basis_influences.size(); i++){
        basis_influences[i].clear();
    }
    basis_influences.clear();
    normalising_coefficients.clear();

    BasisInfluence basis_influence;
    double normalising_denominator;

    for (double u = 0; u <= 1.0; u+= u_increment){
        //add the vector that will hold all the basis influences for this one u value
        basis_influences.push_back(vector<BasisInfluence>());
        normalising_denominator = 0;
        //loop through the basis functions to find all the influences
        for (int i = 0; i < basis_functions.size(); i++)
        {
            basis_influence.basis_scalar = cheatFunc(i,u) * weights[i];
            basis_influence.basis_index = i;
            normalising_denominator += basis_influence.basis_scalar;
            basis_influences[basis_influences.size()-1].push_back(basis_influence);
        }
        //add the normalising coefficient to the list
        if (normalising_denominator != 0){
            normalising_coefficients.push_back(1/normalising_denominator);
        } else {
            normalising_coefficients.push_back(0);
        }
    }

    updateCurvePoints();
}

void Pup::updateCurvePoints()
{
    curve_points.clear();
    Point cur_point;

    //add a new point for each outer vector in basis_influences
    for (int i = 0; i < basis_influences.size(); i++)
    {
        //if there is no basis functions influencing this point, what should we do? right now im just not adding any points
        if (basis_influences[i].size() == 0){
            continue;
        }
        //if the normalising coefficient would make everything have no influence, skip this point (this happens when there is no basis functions with influence)
        if (normalising_coefficients[i] == 0){
            continue;
        }

        cur_point = Point(0,0,1);
        //multiply each basis influence by the corresponding control point
        for (int j = 0; j < basis_influences[i].size(); j++){
            cur_point = cur_point + control_points[basis_influences[i][j].basis_index]*basis_influences[i][j].basis_scalar;
        }
        cur_point = cur_point*normalising_coefficients[i];
        curve_points.push_back(cur_point);
    }
}

double Pup::bFunc(int i, double u)
{
    if ((u > basis_functions[i].left_most.x)&&(u < basis_functions[i].right_most.x)){
        // PROPER CODE, DOESN'T WORK FOR ADDING BASIS CONTROL POINTS DUE TO STANDARD KNOT SEQUENCE
        double basis_u = (u-basis_functions[i].left_most.x)/(basis_functions[i].right_most.x - basis_functions[i].left_most.x);
        return basis_functions[i].getSinglePoint(basis_u).y;
    }
    return 0;
}

//SPEEDS UP BASIS INFLUENCE CALCULATIONS AND SOLVES THE CONTROL POINT ADDING ISSUE
double Pup::cheatFunc(int i, double u)
{
    if (i != last_i){
        last_ci = 0;
    }

    if ((u > basis_functions[i].left_most.x)&&(u < basis_functions[i].right_most.x))
    {
        //SOLUTION 2 FOR ADDING BASIS CONTROL POINTS (SEE THE ADDBASISCONTROLPOINT FUNCTION)
        for (int j = last_ci; j < basis_functions[i].curve_points.size()-1; j++){
            if ((basis_functions[i].curve_points[j].x <= u)&&((basis_functions[i].curve_points[j+1].x >= u))){
                //fastest:
                return basis_functions[i].curve_points[j].y;
                //most accurate:
                //double y_diff = basis_functions[i].curve_points[j+1].y - basis_functions[i].curve_points[j].y;
                //return (basis_functions[i].curve_points[j].y + (u-basis_functions[i].curve_points[j].x)/y_diff);
            }
        }
    }
    return 0;
}

void Pup::setupDefaultBasis()
{
    //the weights are generated by default to 1.0
    vector<Point> temp_control_points = vector<Point>();
    temp_control_points.push_back(Point(0,0,0));
    temp_control_points.push_back(Point(0.25,0,0));
    temp_control_points.push_back(Point(0.5,1,0));
    temp_control_points.push_back(Point(0.75,0,0));
    temp_control_points.push_back(Point(1.0,0,0));

    default_basis = Nurbs(temp_control_points,vector<double>(),3,false);
}

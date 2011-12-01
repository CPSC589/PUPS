#include "pup.h"

Pup::Pup()
{
    control_points = vector<Point>();
    curve_points = vector<Point>();
    basis_centers = vector<double>();
    basis_functions = vector<PupBasis>();
    weights = vector<double>();
}

Pup::Pup(vector<Point> _control_points, vector<PupBasis> _basis_functions, vector<double> _weights)
{
    control_points = _control_points;
    basis_functions = _basis_functions;
    weights = _weights;

    //auto-population of default weights if necessary
    if (control_points.size() != weights.size()){
        weights.resize(control_points.size(), 1.0);
    }

    curve_points = vector<Point>();
    basis_centers = vector<double>();

    updateAll();
}

Pup::~Pup(){

}

void Pup::addControlPoint(Point _cp, PupBasis _basis, double weight){
    control_points.push_back(_cp);
    basis_functions.push_back(_basis);
    weights.push_back(weight);
    updateAll();
}

Point Pup::getSinglePoint(double u)
{
    Point s = Point();
    double normalising_denominator = 0;

    for (int i = 0; i < control_points.size(); i++)
    {
        double cp_coefficient = bFunc(i,u) * weights[i];
        s = s + control_points[i] * cp_coefficient;
        normalising_denominator += cp_coefficient;
    }

    s = s / normalising_denominator;
    return s;
}

void Pup::updateAll(){

    basis_centers.clear();

    if (control_points.size() == 0){
        return;
    } else if (control_points.size() == 1){
        basis_centers.push_back(0.5);
        basis_functions[0].actual_radius_left = 0.5;
        basis_functions[0].actual_radius_right = 0.5;
        return;
    }

    //first and last control point basis functions are centered at 0 and 1
    double step_denominator = control_points.size()-1;
    for (int i = 0; i <= step_denominator; i++){
        basis_centers.push_back((i/step_denominator) + basis_functions[i].center_offset);
    }

    //update the radii of all the basis functions
    for (int i = 0; i < basis_functions.size(); i++){
        if (basis_functions[i].is_relative_radius){
            basis_functions[i].actual_radius_left = basis_functions[i].relative_radius_left*(1/step_denominator);
            basis_functions[i].actual_radius_right = basis_functions[i].relative_radius_right*(1/step_denominator);
        }
    }

    updateCurve();
}

void Pup::updateCurve()
{
    curve_points.clear();
    for (double u = 0; u < 1.0; u+= 0.01){
        curve_points.push_back(getSinglePoint(u));
    }
}

double Pup::bFunc(int i, double u)
{

    //check if this control point should have influence
    if ((u >= basis_centers[i]-basis_functions[i].actual_radius_left) && (u <= basis_centers[i]+basis_functions[i].actual_radius_right))
    {
        // find the difference between u and the beginning of the basis function
        double distance_into_range = u - (basis_centers[i]-basis_functions[i].actual_radius_left);
        //get the u value to send to the basis curve
        double basis_u = distance_into_range / (basis_functions[i].actual_radius_left + basis_functions[i].actual_radius_right);

        return basis_functions[i].basis_function.getSinglePoint(basis_u).y;
    }
    else {
        return 0;
    }
}

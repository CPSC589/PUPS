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
    //z value will hold info about normalising_denominator
    Point s = Point(0,0,1);
    double normalising_denominator = 0;

    for (int i = 0; i < control_points.size(); i++)
    {
        double cp_coefficient = bFunc(i,u) * weights[i];
        s = s + control_points[i] * cp_coefficient;
        normalising_denominator += cp_coefficient;
    }
    if (normalising_denominator == 0)
        return Point(0.0,0.0,-1);
    s = s / normalising_denominator;
    return s;
}

void Pup::updateAll(){

    basis_centers.clear();

    if (control_points.size() == 0){
        return;
    } else if (control_points.size() == 1){
        basis_centers.push_back(0.5);
        if (basis_functions[0].is_relative_radius){
            basis_functions[0].actual_radius_left = 0.5;
            basis_functions[0].actual_radius_right = 0.5;
        }
        return;
    }

    float first_center;
    double step_numerator = 1.0;
    double step_denominator = control_points.size()-1;

    if (basis_functions[basis_functions.size()-1].is_relative_radius){
        step_denominator += basis_functions[basis_functions.size()-1].relative_radius_right;
    } else {
        step_numerator -= basis_functions[basis_functions.size()-1].actual_radius_right;
    }

    if (basis_functions[0].is_relative_radius){
        step_denominator += basis_functions[0].relative_radius_left;
        first_center = basis_functions[0].relative_radius_left*step_numerator/step_denominator;
    } else {
        step_numerator -= basis_functions[0].actual_radius_left;
        first_center = basis_functions[0].actual_radius_left;
    }

    for (int i = 0; i < basis_functions.size(); i++)
    {
        basis_centers.push_back(first_center + i*(step_numerator/step_denominator) + basis_functions[i].center_offset);

        if (basis_functions[i].is_relative_radius){
            basis_functions[i].actual_radius_left = basis_functions[i].relative_radius_left*(step_numerator/step_denominator);
            basis_functions[i].actual_radius_right = basis_functions[i].relative_radius_right*(step_numerator/step_denominator);
        }
    }

    updateCurve();
}

void Pup::updateCurve()
{
    curve_points.clear();
    for (double u = 0; u < 1.0; u+= 0.001){
        Point temp = getSinglePoint(u);
        if (temp.z != -1)
            curve_points.push_back(temp);
    }
}

double Pup::bFunc(int i, double u)
{
    double dist_from_center = u - basis_centers[i];

    if (dist_from_center < 0)
    {
        dist_from_center *= -1.0;
        if (dist_from_center <= basis_functions[i].actual_radius_left){
            double basis_u = 0.5 - 0.5*dist_from_center/basis_functions[i].actual_radius_left;
            return basis_functions[i].basis_function.getSinglePoint(basis_u).y;
        }
    } else {
        if (dist_from_center <= basis_functions[i].actual_radius_right){
            double basis_u = 0.5 + 0.5*dist_from_center/basis_functions[i].actual_radius_right;
            return basis_functions[i].basis_function.getSinglePoint(basis_u).y;
        }
    }

    return 0;

    /*
    //check if this control point should have influence
    if ((u >= basis_centers[i]-basis_functions[i].actual_radius_left) && (u <= basis_centers[i]+basis_functions[i].actual_radius_right))
    {
        double dist_from_center = u - basis_centers[i];

        // find the difference between u and the beginning of the basis function
        double distance_into_range = u - (basis_centers[i]-basis_functions[i].actual_radius_left);
        //get the u value to send to the basis curve
        double basis_u = distance_into_range / (basis_functions[i].actual_radius_left + basis_functions[i].actual_radius_right);

        return basis_functions[i].basis_function.getSinglePoint(basis_u).y;
    }
    else {
        return 0;
    } */
}

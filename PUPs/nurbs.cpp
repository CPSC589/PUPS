#include "nurbs.h"

float NURBS_U_VAL = 0.001;
Nurbs::Nurbs(vector<Point> _control_points, vector<double> _weights, int _order, bool _uniform)
{
    control_points = _control_points;
    weights = _weights;

    //auto-population of default weights if necessary
    if (control_points.size() != weights.size()){
        weights.resize(control_points.size(), 1.0);
    }

    knots = vector<double>();
    curve_points = vector<Point>();
    order = _order;
    uniform = _uniform;

    left_most = bottom_most = Point(100000,100000,0);
    right_most = top_most = Point(-100000, -100000, 0);

    updateAll();
}

Nurbs::~Nurbs(){}

void Nurbs::updateAll()
{
    knots.clear();
    double step_denominator = 0;

    //generate knots
    //won't work properly if order is more than num control points
    if (uniform)
    {
        step_denominator = (control_points.size()+order);
        for (int i = 0; i <= step_denominator; i++){
            knots.push_back(i/step_denominator);
        }
    }
    else {
        step_denominator = (control_points.size() - order + 1);
        for (int i = 0; i < order; i++){
            knots.push_back(0);
        }
        for (int i = 0; i < control_points.size() - order; i++){
            knots.push_back((double)(i+1)/step_denominator);
        }
        for (int i = 0; i < order; i++){
            knots.push_back(1);
        }
    }

    updateCurve();
}

void Nurbs::updateCurve()
{
    curve_points.clear();

    if (uniform){
         for (double u = knots[order-1]; u <= knots[control_points.size()]; u+=NURBS_U_VAL){
             curve_points.push_back(getSinglePoint(u));
         }
    }
    else {
        for (double u = 0; u <= 1; u+=NURBS_U_VAL) {
            curve_points.push_back(getSinglePoint(u));
        }
    }
}

Point Nurbs::getSinglePoint(double u)
{
    //if uniform, u is only defined from u_k-1 to u_m+1, so illegal u values will be replaced with closest end knots
    //for standard, u must fall within 0 and 1
    if (uniform)
    {
        if (u < knots[order-1]) u = knots[order-1];
        else if (u > knots[control_points.size()]) u = knots[control_points.size()];
    } else {
        if (u < 0) u = 0;
        else if (u > 1) u = 1.0;
    }

    Point s = Point();
    double normalising_denominator = 0;

    for (int i = 0; i < control_points.size(); i++)
    {
        double cp_coefficient = nFunc(i,u,order,0) * weights[i];
        s = s + control_points[i] * cp_coefficient;
        normalising_denominator += cp_coefficient;
    }

    s = s / normalising_denominator;

    if (s.x > right_most.x){
        right_most = s;
    } else if (s.x < left_most.x){
        left_most = s;
    }

    if (s.y > top_most.y){
        top_most = s;
    } else if (s.y < bottom_most.y){
        bottom_most = s;
    }

    return s;
}

double Nurbs::nFunc(int i, double u, int order, int depth)
{
    if (order > 1){

        double denominator1 = knots[i+order-1] - knots[i];
        double denominator2 = knots[i+order]-knots[i+1];

        double term1,term2;
        if (denominator1 == 0){
            term1 = 0;
        } else {
            term1 = (u-knots[i])/denominator1;
        }
        if (denominator2 == 0){
            term2 = 0;
        } else {
            term2 = (knots[i+order]-u)/denominator2;
        }

        return term1*nFunc(i,u,order-1,(depth+1)) + term2*nFunc(i+1,u, order-1,(depth+1));

    } else {
        if ((u >= knots[i])&&(u < knots[i+1])){
            return 1;
        } else {
            return 0;
        }
    }
}

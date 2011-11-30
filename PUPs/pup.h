#ifndef PUP_H
#define PUP_H

#include <vector>
#include "point.h"
#include "pupbasis.h"

class Pup{
public:
    Pup();
    Pup(vector<Point> _control_points, vector<PupBasis> _basis_functions, vector<double> _weights = vector<double>());
    ~Pup();

    vector<Point> control_points;
    vector<double> weights;
    vector<Point> curve_points;
    vector<double> basis_centers;
    vector<PupBasis> basis_functions;

    //ease of use functions
    void addControlPoint(Point _cp, PupBasis _basis, double weight = 1);


    //call this only if control points have moved or weights have changed
    void updateCurve();
    //call this if a basis function has changed
    void updateAll();
    //call this for a single point on the PUP curve
    Point getSinglePoint(double u);

private:
    double bFunc(int i, double u);
};




/*
// PUP PART 2:
//NURBS CLASS WITH SUPPORT FOR WEIGHTS, BUT NOT RADII OR MULTIPLE ORDERS

class Pup{
public:
    static void generateCurve(vector<Point>* in, vector<Point>* out, vector<double>* radii, int order, vector<double>* weights)
    {
        out->clear();
        bool uniform = true;
        vector<double> basis_centers = vector<double>();
        double step_denominator = 0;

        if (uniform)
        {
            // generate basis_centers
            step_denominator = (in->size()+order);
            for (int i = 0; i <= step_denominator; i++){
                basis_centers.push_back(i/step_denominator);
            }

            for (double u = basis_centers[order-1]; u <= basis_centers[in->size()]; u+=0.01)
            {
                Point s = Point();
                double normalising_denominator = 0;

                for (int i = 0; i < in->size(); i++)
                {
                    double cp_coefficient = nFunc(i,order,u, &basis_centers, 0) * weights->at(i);
                    s = s + in->at(i) * cp_coefficient;
                    normalising_denominator += cp_coefficient;
                }

                s = s / normalising_denominator;
                out->push_back(s);
            }
        }
        else
        {
            //generate basis_centers
            step_denominator = (in->size() - order + 1);
            for (int i = 0; i < order; i++){
                basis_centers.push_back(0);
            }
            for (int i = 0; i < in->size() - order; i++){
                basis_centers.push_back((double)(i+1)/step_denominator);
            }
            for (int i = 0; i < order; i++){
                basis_centers.push_back(1);
            }

            //generate curve points
            for (double u = 0; u <= 1; u+=0.01)
            {
                Point s = Point();
                double normalising_denominator = 0;

                for (int i = 0; i < in->size(); i++){
                    double cp_coefficient = nFunc(i,order,u, &basis_centers, 0) * weights->at(i);
                    s = s + in->at(i) * cp_coefficient;
                    normalising_denominator += cp_coefficient;
                }

                s = s / normalising_denominator;
                out->push_back(s);
            }
        }
    }

private:
    static double nFunc(int i, int order, double u, vector<double>* basis_centers, int depth){
        if (order>1){

            double denominator1 = basis_centers->at(i+order-1) - basis_centers->at(i);
            double denominator2 = basis_centers->at(i+order)-basis_centers->at(i+1);

            double term1,term2;
            if (denominator1 == 0){
                term1 = 0;
            } else {
                term1 = (u-basis_centers->at(i))/denominator1;
            }
            if (denominator2 == 0){
                term2 = 0;
            } else {
                term2 = (basis_centers->at(i+order)-u)/denominator2;
            }

            return term1*nFunc(i,order-1,u,basis_centers,(depth+1)) + term2*nFunc(i+1,order-1,u,basis_centers,(depth+1));

        } else {
            if ((u >= basis_centers->at(i))&&(u < basis_centers->at(i+1))){
                return 1;
            } else {
                return 0;
            }
        }
    }
};

*/

/* PUP CODE STEP 1:
- B-SPLINE (different orders not really supported, radii not used) WITH STANDARD OR UNIFORM KNOT SEQUENCES
- NO CHECKS FOR INVALID ORDER/CONTROL_POINT COMBINATIONS (ie 3 control points with order 4)

class Pup{
public:
    static void generateCurve(vector<Point>* in, vector<Point>* out, vector<double>* radii, vector<int>* orders)
    {
        bool uniform = true;

        out->clear();
        vector<double> basis_centers = vector<double>();

        if (uniform)
        {
            // generate basis_centers
            double step_denominator = (in->size()+orders->at(orders->size()-1));
            for (int i = 0; i <= step_denominator; i++){
                basis_centers.push_back(i/step_denominator);
            }

            //generate points (parameter range is defined here)
            for (double u = basis_centers[orders->at(0)-1]; u <= basis_centers[in->size()]; u+=0.01)
            {
                Point s = Point();

                for (int i = 0; i < in->size(); i++){
                    s = s + in->at(i) * nFunc(i,orders->at(i),u, &basis_centers, 0);
                }

                out->push_back(s);
            }
        }
        else
        {
            //generate basis_centers
            double step_denominator = (in->size()-orders->at(orders->size()-1)+1);
            for (int i = 0; i < orders->at(0); i++){
                basis_centers.push_back(0);
            }
            for (int i = 0; i < in->size() - orders->at(orders->size()-1); i++){
                basis_centers.push_back((double)(i+1)/step_denominator);
            }
            for (int i = 0; i < orders->at(orders->size()-1); i++){
                basis_centers.push_back(1);
            }

            //generate curve points
            for (double u = 0; u <= 1; u+=0.01)
            {
                Point s = Point();

                for (int i = 0; i < in->size(); i++){
                    s = s + in->at(i) * nFunc(i,orders->at(i),u, &basis_centers, 0);
                }
                out->push_back(s);
            }
        }
    }

private:
    static double nFunc(int i, int order, double u, vector<double>* basis_centers, int depth){
        if (order>1){

            double denominator1 = basis_centers->at(i+order-1) - basis_centers->at(i);
            double denominator2 = basis_centers->at(i+order)-basis_centers->at(i+1);

            double term1,term2;
            if (denominator1 == 0){
                term1 = 0;
            } else {
                term1 = (u-basis_centers->at(i))/denominator1;
            }
            if (denominator2 == 0){
                term2 = 0;
            } else {
                term2 = (basis_centers->at(i+order)-u)/denominator2;
            }

            return term1*nFunc(i,order-1,u,basis_centers,(depth+1)) + term2*nFunc(i+1,order-1,u,basis_centers,(depth+1));

        } else {
            if ((u >= basis_centers->at(i))&&(u < basis_centers->at(i+1))){
                return 1;
            } else {
                return 0;
            }
        }
    }
};
*/

#endif // PUP_H

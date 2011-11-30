#ifndef NURBS_H
#define NURBS_H

#include <vector>
#include  "point.h"

class Nurbs{
public:
    Nurbs(vector<Point> _control_points = vector<Point>(), vector<double> _weights = vector<double>(), int _order = 3, bool _uniform = true);
    ~Nurbs();

    vector<Point> control_points;
    vector<Point> curve_points;
    vector<double> weights;
    vector<double> knots;
    bool uniform;
    int order;

    //call this if the knot sequence doesn't need to be updated
    void updateCurve();
    //call this if the knot sequence (and by extension the curve) needs to be updated
    void updateAll();
    //call this for a single point along the curve
    Point getSinglePoint(double u);

    Point left_most;
    Point right_most;
    Point top_most;
    Point bottom_most;

private:
    double nFunc(int i, double u, int order, int depth = 0);
};


#endif // NURBS_H

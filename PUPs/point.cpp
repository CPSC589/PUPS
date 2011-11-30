//Andrew Legault
//Oct.4/09
//CPSC 453

//Also used for CPSC 589 in 2011

#include "Point.h"
#include <cmath>
using namespace std;

//Constructors
Point::Point(){
	x=0.0;
	y=0.0;
	z=0.0;
}

Point::Point(float x_in, float y_in, float z_in){
	x=x_in;
	y=y_in;
	z=z_in;
}

//Vector-Point Addition
Point Point::operator +(Point a) const {
	return Point(x+a.x, y+a.y, z+a.z);
}

//Point-Point Subtraction
Point Point::operator -(Point a) const {
	return Point(x-a.x, y-a.y, z-a.z);
}

//Scalar Division
Point Point::operator /(float s) const {
	return Point(x/s, y/s, z/s);
}

//Scalar Multiplication
Point Point::operator *(float s) const {
	return Point(x*s, y*s, z*s);
}

//Scalar Multiplication
bool Point::equals(Point p2){
	if (x != p2.x){
		return false;
	}
	if (y != p2.y){
		return false;
	}
	if (z != p2.z){
		return false;
	}
	return true;
}

void Point::abs(){
	x = fabs(x);
	y = fabs(y);
	z = fabs(z);
}

//Vector cross-multiplication
Point Point::cross(Point v2){
	return Point(y*(v2.z)-z*(v2.y), z*(v2.x)-x*(v2.z), x*(v2.y)-y*(v2.x));
}

//Vector dot product
float Point::dot(Point v2){
	return (x*v2.x + y*v2.y + z*v2.z);
}

//Vector normalisation
void Point::normalise(){
	float length = magnitude();
	x /= length;
	y /= length;
	z /= length;
}

/* NEEDS TO BE CHANGED TO 3D ROTATION
//Vector Rotation
void Point::rotate(float rads){
	float newX = cos(rads)*x - sin(rads)*y;
	float newY = sin(rads)*x + cos(rads)*y;
	x = newX;
	y = newY;
}
*/

//Zeros out a point/vector
void Point::zero(){
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

//Returns magnitude of a vector
float Point::magnitude(){
	return sqrt(x*x+y*y+z*z);
}

//for printing point info to console
ostream& operator << (ostream& output, const Point& p)
{
	output << p.x << ", " << p.y << ", " << p.z << "\n";
	return output;
}

//send point to output file stream
ofstream& operator << (ofstream& output, const Point& p)
{
	output << p.x << " " << p.y << " " << p.z << "\n";
	return output;
}

//get point from input file stream
ifstream& operator >> (ifstream& input, Point& p)
{
	input >> p.x >> p.y >> p.z;
	return input;
}

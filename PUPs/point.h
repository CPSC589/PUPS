//Andrew Legault
//Oct.4/09
//CPSC 453

//Also used in CPSC 589 in 2011

#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <fstream>
using namespace std;

class Point{
	public:
	//file stream IO
	friend ostream& operator << (ostream& output, const Point& p);
	friend ofstream& operator << (ofstream& output, const Point& p);
	friend ifstream& operator >> (ifstream& input, Point& p);        

	//constructors
	float x,y,z;
	Point();
	Point(float, float, float);
        ~Point();

	//operator overloading operations
	Point operator +(Point) const;
	Point operator -(Point) const;
	Point operator *(float) const;
	Point operator /(float) const;
	
	//other operations
	Point cross(Point);	
	float dot(Point);
	//void rotate(float);
	void normalise();
	void zero();
	void abs();
	float magnitude();
	bool equals(Point);

        //Print
        void print();
};

#endif

#ifndef FILEIO_H
#define FILEIO_H

#include "controlpoint.h"
#include "pup.h"
#include <iostream>
#include <fstream>

class FileIO
{
public:
    FileIO();
    void saveData(vector<ControlPoint> ControlPoints, vector<PupBasis> basisFunctions, vector<double> basisCentres, string fileName);
    vector<ControlPoint> loadData(string fileName);
};

#endif // FILEIO_H

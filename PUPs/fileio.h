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

    void saveData(Nurbs defaultBasis, double defaultWeight, double uInc, vector<ControlPoint> ControlPoints, vector<Nurbs> basisFunctions, string fileName);
    Pup loadData(string fileName);

    void saveCollection(vector<Nurbs> BasisCollection, string fileName);
    vector<Nurbs> loadCollection();
};

#endif // FILEIO_H

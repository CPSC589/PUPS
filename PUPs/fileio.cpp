#include "fileio.h"

FileIO::FileIO()
{
}

void FileIO::saveData(vector<ControlPoint> ControlPoints, vector<PupBasis> basisFunctions, vector<double> basisCentres, string fileName){
    ofstream outFile;
    outFile.open((fileName + ".cps").c_str());
    if (outFile.is_open()) {
        outFile << "ControlPoints\n"; // Saving the control point information
        for(int i = 0; i < ControlPoints.size(); i++){
            ControlPoint currentPoint = ControlPoints[i];
            GLfloat weight = currentPoint.getWeight();
            float x = currentPoint.x;
            float y = currentPoint.y;
            float z = currentPoint.z;
            outFile << weight << ' ' << x << ' ' << y << ' ' << z << '\n';
        }
        outFile << "BasisFunctions\n"; // Saving the Basis Function information
        for(int i = 0; i < basisFunctions.size(); i++){
            PupBasis currentBasis = basisFunctions[i];
            double actualRL = currentBasis.actual_radius_left;
            double actualRR = currentBasis.actual_radius_right;
            Nurbs nurb = currentBasis.basis_function;
            double centerOffset = currentBasis.center_offset;
            bool isRelativeRadius = currentBasis.is_relative_radius;
            double relativeRL = currentBasis.relative_radius_left;
            double relativeRR = currentBasis.relative_radius_right;

            // Nurbs want ControlPoints, Weights, Order, Uniform
            outFile << "NewBasis\n";
            outFile << actualRL << " " << actualRR << " " << centerOffset << " " << isRelativeRadius << " "
                    << relativeRL << " " << relativeRR << " "
                    << nurb.order << " " << nurb.uniform << '\n';
            vector<Point> nurbPoints = nurb.control_points;
            vector<double> nurbWeights = nurb.weights;
            for(int j = 0; j < nurbPoints.size(); j++){
                Point currentPoint = nurbPoints[j];
                double weight = nurbWeights[j];
                outFile << weight << ' ' << currentPoint.x << ' ' << currentPoint.y << ' ' << currentPoint.z << '\n';
            }
        }

        outFile << "BasisCentres\n";   // Saving the Basis Centres
        for(int i = 0; i < basisCentres.size(); i++){
            outFile << basisCentres[i] << '\n';
        }

        outFile.close();
    }
    else{
        cout << "Failed to open file to write to.";
    }

}

Pup FileIO::loadData(string fileName){
    vector<Point> ControlPoints;
    vector<PupBasis> BasisFunctions;
    vector<double> BasisCenters;
    vector<double> Weights;
    ifstream infile;
    infile.open((fileName).c_str());
    string currentLine;
    if(infile.is_open()){
        getline(infile, currentLine); getline(infile, currentLine);
        while(currentLine.compare("BasisFunctions") != 0){ // Parse ControlPoints
            string parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
            double currentWeight = atof(parseLine.c_str());
            currentLine = currentLine.substr(currentLine.find_first_of(' ')+1); parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
            float x = atof(parseLine.c_str());
            currentLine = currentLine.substr(currentLine.find_first_of(' ')+1); parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
            float y = atof(parseLine.c_str());
            currentLine = currentLine.substr(currentLine.find_first_of(' ')+1); parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
            float z = atof(parseLine.c_str());
            Weights.push_back(currentWeight);
            ControlPoints.push_back(Point(x, y, z));
            qDebug() << currentWeight << ' ' << x << ' ' << y << ' ' << z << '\n';
            getline(infile, currentLine);
        }
        getline(infile, currentLine);
        vector<Point> NurbPoints;
        while(currentLine.compare("BasisCentres") != 0){
            string previous = currentLine;
            getline(infile, currentLine);
            while(currentLine.compare("NewBasis") != 0){
                if(previous.compare("NewBasis") == 0){

                }
                else{
                    string parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
                    double currentWeight = atof(parseLine.c_str());
                    currentLine = currentLine.substr(currentLine.find_first_of(' ')+1); parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
                    float x = atof(parseLine.c_str());
                    currentLine = currentLine.substr(currentLine.find_first_of(' ')+1); parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
                    float y = atof(parseLine.c_str());
                    currentLine = currentLine.substr(currentLine.find_first_of(' ')+1); parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
                    float z = atof(parseLine.c_str());
                    Weights.push_back(currentWeight);

                    NurbPoints.push_back(Point(x, y, z));
                }
            }
            //BasisFunctions.push_back(currentBasis);
        }

        infile.close();
    }


    Pup pupCurve = Pup(ControlPoints, BasisFunctions, Weights);
    pupCurve.basis_centers = BasisCenters;
    return pupCurve;
}

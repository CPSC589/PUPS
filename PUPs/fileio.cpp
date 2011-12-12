#include "fileio.h"
#include <iostream>
using namespace std;

FileIO::FileIO()
{
}

// Saves the current state of the program to a file
void FileIO::saveData(Nurbs defaultBasis, double defaultWeight, double uInc, vector<ControlPoint> ControlPoints, vector<Nurbs> basisFunctions, string fileName){
    ofstream outFile;
    outFile.open((fileName + ".pups").c_str());
    if (outFile.is_open()) {
        outFile << "DefaultBasis\n"; // Saving the default Basis and default Weight information
        outFile << defaultWeight << ' ' << uInc << ' ' << defaultBasis.order << ' ' << defaultBasis.uniform << '\n';
        vector<Point> defaultPoints = defaultBasis.control_points;
        vector<double> defaultWeights = defaultBasis.weights;
        for(int j = 0; j < defaultPoints.size(); j++){
            Point currentPoint = defaultPoints[j];
            double weight = defaultWeights[j];
            outFile << weight << ' ' << currentPoint.x << ' ' << currentPoint.y << ' ' << currentPoint.z << '\n';
        }

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
            Nurbs nurb = basisFunctions[i];

            // Nurbs want ControlPoints, Weights, Order, Uniform
            outFile << "NewBasis\n";
            outFile << nurb.order << ' ' << nurb.uniform << '\n';
            vector<Point> nurbPoints = nurb.control_points;
            vector<double> nurbWeights = nurb.weights;
            for(int j = 0; j < nurbPoints.size(); j++){
                Point currentPoint = nurbPoints[j];
                double weight = nurbWeights[j];
                outFile << weight << ' ' << currentPoint.x << ' ' << currentPoint.y << ' ' << currentPoint.z << '\n';
            }
        }

        outFile << "EndOfFile";
        outFile.close();
    }
    else{
        cout << "Failed to open file to write to.";
    }

}

// Used to load the information that will be used to redraw a particular state the the system
// was in when the program saved.
Pup FileIO::loadData(string fileName){
    vector<Point> ControlPoints;
    vector<double> Weights;
    vector<Point> defaultCPs;
    vector<double> defaultWeights;
    vector<Nurbs> basisFunctions;
    Nurbs defaultBasis;
    double defaultWeight;
    double uInc;
    ifstream infile;
    infile.open((fileName).c_str());
    string currentLine;
    if(infile.is_open()){
        getline(infile, currentLine); getline(infile, currentLine);
        string parse = currentLine.substr(0, currentLine.find_first_of(' '));
        defaultWeight = atof(parse.c_str());
        currentLine = currentLine.substr(currentLine.find_first_of(' ')+1); parse = currentLine.substr(0, currentLine.find_first_of(' '));
        uInc = atof(parse.c_str());
        currentLine = currentLine.substr(currentLine.find_first_of(' ')+1); parse = currentLine.substr(0, currentLine.find_first_of(' '));
        int defaultOrder = atoi(parse.c_str());
        currentLine = currentLine.substr(currentLine.find_first_of(' ')+1); parse = currentLine.substr(0, currentLine.find_first_of(' '));
        bool defaultUniform = atoi(parse.c_str());
        getline(infile, currentLine);
        while(currentLine.compare("ControlPoints") != 0){ // Parse Default Basis
            string parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
            double currentWeight = atof(parseLine.c_str());
            currentLine = currentLine.substr(currentLine.find_first_of(' ')+1); parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
            float x = atof(parseLine.c_str());
            currentLine = currentLine.substr(currentLine.find_first_of(' ')+1); parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
            float y = atof(parseLine.c_str());
            currentLine = currentLine.substr(currentLine.find_first_of(' ')+1); parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
            float z = atof(parseLine.c_str());
            defaultWeights.push_back(currentWeight);
            defaultCPs.push_back(Point(x, y, z));
            getline(infile, currentLine);
        }
        defaultBasis = Nurbs(defaultCPs, defaultWeights, defaultOrder, defaultUniform);
        getline(infile, currentLine);
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
            getline(infile, currentLine);
        }

        getline(infile, currentLine);
        while(currentLine.compare("EndOfFile") != 0){ // Until end of file, gather remaining basis info
            string previous = currentLine;
            getline(infile, currentLine);
            vector<Point> NurbPoints = vector<Point>();
            vector<double> nurbWeights = vector<double>();
            int order;
            bool uniform;
            string parseLine;
            while(currentLine.compare("NewBasis") != 0 && currentLine.compare("EndOfFile") != 0){ // Gather basis info
                if(previous.compare("NewBasis") == 0){ // Get nurb info
                   parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
                   order = atoi(parseLine.c_str());
                   currentLine = currentLine.substr(currentLine.find_first_of(' ')+1); parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
                   uniform = atoi(parseLine.c_str());
                }
                else{ // Get Nurb Control Points
                    parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
                    double currentWeight = atof(parseLine.c_str());
                    currentLine = currentLine.substr(currentLine.find_first_of(' ')+1); parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
                    float x = atof(parseLine.c_str());
                    currentLine = currentLine.substr(currentLine.find_first_of(' ')+1); parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
                    float y = atof(parseLine.c_str());
                    currentLine = currentLine.substr(currentLine.find_first_of(' ')+1); parseLine = currentLine.substr(0, currentLine.find_first_of(' '));
                    float z = atof(parseLine.c_str());
                    nurbWeights.push_back(currentWeight);
                    NurbPoints.push_back(Point(x, y, z));
                }
                previous = currentLine;
                getline(infile, currentLine);
            }

            Nurbs currentNurb = Nurbs(NurbPoints, nurbWeights, order, uniform);
            basisFunctions.push_back(currentNurb);

        }

        infile.close();
    }

    Pup pupCurve = Pup(defaultBasis, defaultWeight, uInc, ControlPoints, basisFunctions, Weights);
    return pupCurve;
}

void FileIO::saveCollection(vector<Nurbs> BasisCollection, string fileName){
    ofstream outFile;
    outFile.open((fileName + ".col").c_str());
    if (outFile.is_open()) {
        for(int i = 0; i < BasisCollection.size(); i++){
            Nurbs nurb = BasisCollection[i];

            // Nurbs want ControlPoints, Weights, Order, Uniform
            outFile << "NewBasis\n";
            outFile << nurb.order << ' ' << nurb.uniform << '\n';
            vector<Point> nurbPoints = nurb.control_points;
            vector<double> nurbWeights = nurb.weights;
            for(int j = 0; j < nurbPoints.size(); j++){
                Point currentPoint = nurbPoints[j];
                double weight = nurbWeights[j];
                outFile << weight << ' ' << currentPoint.x << ' ' << currentPoint.y << ' ' << currentPoint.z << '\n';
            }
        }


        outFile.close();
    }
}

vector<Nurbs> FileIO::loadCollection(){
    vector<Nurbs> BasisCollection;

    return BasisCollection;
}

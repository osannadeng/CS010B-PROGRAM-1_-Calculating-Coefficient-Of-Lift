#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
using namespace std;

//prototypes
void readData(const string &, vector<double> &, vector<double> &);
double interpolation(double, const vector<double> &, const vector<double> &);
bool isOrdered(const vector<double> &);
void reorder(vector<double> &, vector<double> &);

int main(int argc, char *argv[]) {
    vector<double> angle;
    vector<double> coeff;
    string fileName;
    double currAngle;
    double linInterpolation;

    fileName = argv[1];

    readData(fileName, angle, coeff);

    bool cont = true;
    string userIn;
    do {
        cout << "Enter a flight-path angle: ";
        cin >> currAngle;
        cout << endl;

        if (currAngle >= angle.at(0) && currAngle <= angle.at(angle.size() - 1)) {
            linInterpolation = interpolation(currAngle, angle, coeff);
            cout << linInterpolation << endl;
        } else {
            cout << "ERROR: Angle is out of bounds." << endl;
        }

        cout << "Would you like to enter another flight-path angle? ";
        cin >> userIn;
        cout << endl;

        if (userIn == "Yes") {
            cont = true;
        } else {
            cont = false;
        }
    } while (cont == true);
    
    return 0;
}

//data from file to vectors
void readData(const string &file, vector<double> &angle, vector<double> &coeff) {
    ifstream inFS;
    double temp;
    inFS.open(file);

    //error code if fails to open
    if (!inFS.is_open()) {
        cout << "Error opening " << file << endl;
        exit(EXIT_FAILURE);
    }

    //uploading to vectors
    while (inFS >> temp) {
        angle.push_back(temp);
        inFS >> temp;
        coeff.push_back(temp);
    }

    inFS.close();

    //check order
    bool inOrder = isOrdered(angle);

    //sort if not in order
    if (inOrder == false) {
        reorder(angle, coeff);
    }
}

double interpolation(double currAngle, const vector<double> &angle, const vector<double> &coeff) {
    int uppBound = angle.at(0);
    double newCoeff;
    //search
    for (unsigned i = 0; i < angle.size(); ++i) {
        if (angle.at(i) == currAngle) {
            return coeff.at(i);
        }
        if (angle.at(i) > currAngle) {
            uppBound = i;
            break;
        }
    }

    int lowBound = uppBound - 1;

    newCoeff = coeff.at(lowBound) + ((currAngle - angle.at(lowBound)) / (angle.at(uppBound) - angle.at(lowBound))) * (coeff.at(uppBound) - coeff.at(lowBound));

    return newCoeff;
}

//check if ordered
bool isOrdered(const vector<double> &angle) {
    double prev;
    for (unsigned i = 1; i < angle.size(); ++i) {
        prev = angle.at(i - 1);
        if (prev > angle.at(i)) {
            return false;
        }
    }
    return true;
}

//sort
void reorder(vector<double> &angle, vector<double> &coeff) {
    double lowest;
    int lowIndex;
    double temp;
    for (unsigned i = 0; i < angle.size(); ++i) {
        lowest = angle.at(i);
        lowIndex = i;
        for (unsigned j = i; j < angle.size(); ++j) {
            if (angle.at(j) < lowest) {
                lowest = angle.at(j);
                lowIndex = j;
            }
        }
        temp = angle.at(i);
        angle.at(i) = lowest;
        angle.at(lowIndex) = temp;
        temp = coeff.at(i);
        coeff.at(i) = coeff.at(lowIndex);
        coeff.at(lowIndex) = temp;
    }
}

#include <cstdlib>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>

using namespace std;

#ifndef HW_DATAGENERATOR_H
#define HW_DATAGENERATOR_H

class DataGenerator {

public:

    // Uniformni generator cisel
    // Parametry:
    //      data - datova sada
    //      solution - suma kazdeho vektoru cisel v datove sade
    void generateData(vector<long> &solution, vector<vector<int8_t>> &data) const;


    // Gaussovsky generator cisel
    // Parametry:
    //      vector - vektor nahodnych cisel
    //      mean - stredni hodnota normalniho rozdelen
    //      sigma - smerodatna odchylka normalniho rozdeleni
    void generateDistribution(vector<int> &vector, const int mean, const int sigma, const int beginIndex) const;

};

#endif //HW_DATAGENERATOR_H

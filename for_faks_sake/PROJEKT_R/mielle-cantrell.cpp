#define _USE_MATH_DEFINES

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

#define BR_DIM 4
#define VEL_POPULACIJE 100

double randomX()
{
    double x = ((double)rand() / RAND_MAX) * 2 - 1;
    return x;
}

vector<double> generirajGenotip()
{
    vector<double> gen;
    for (int i = 0; i < BR_DIM; i++)
    {
        gen.push_back(randomX());
    }
    return gen;
}

class Individual
{
public:
    vector<double> genotip;
    double fitness;
    Individual(vector<double> genotip);
    Individual mutacija(Individual roditelj2);
    double izracunajFitness();
};

Individual::Individual(vector<double> genotipX)
{
    this->genotip = genotipX;
    fitness = izracunajFitness();
};

double Individual::izracunajFitness()
{
    double fitness = 0;
    double clan1 = pow((pow(M_E, -1.0 * genotip[0]) - genotip[1]), 4);
    double clan2 = pow((genotip[1] - genotip[2]), 6);
    double clan3 = pow(tan(genotip[2] - genotip[3]), 4);
    double clan4 = pow(genotip[0], 8);
    fitness = clan1 + 100 * clan2 + clan3 + clan4;

    return fitness;
};

Individual Individual::mutacija(Individual roditelj2){

};

bool operator<(const Individual &ind1, const Individual &ind2)
{
    return ind1.fitness < ind2.fitness;
}

int main()
{
    srand((unsigned)time(NULL));

    vector<Individual> populacija;
    bool pronaden = 0;

    for (int i = 0; i < VEL_POPULACIJE; i++)
    {
        populacija.push_back(Individual(generirajGenotip()));
    }

    while (!pronaden)
    {
        sort(populacija.begin(), populacija.end());

        if (fabs(populacija[0].fitness) < 1E-4)
        {
            pronaden = 1;
            break;
        }
    }
}
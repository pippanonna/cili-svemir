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
#define PRECIZNOST 100

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

Individual Individual::mutacija(Individual roditelj2)
{
    vector<double> kromosom_djeteta;

    for (int i = 0; i < BR_DIM; i++)
    {
        float p = (rand() % (PRECIZNOST + 1)) / PRECIZNOST;
        double aritmSred = (this->genotip[i] + roditelj2.genotip[i]) / 2;

        if (p < 0.9)
        {
            kromosom_djeteta.push_back(aritmSred);
        }
        else
        {
            kromosom_djeteta.push_back(randomX());
        }
    }

    return Individual(kromosom_djeteta);
};

void printGen(Individual jed)
{
    cout << "[";
    for (int i = 0; i < jed.genotip.size(); i++)
    {
        cout << jed.genotip[i] << " ";
    }
    cout << "]";
};

bool operator<(const Individual &ind1, const Individual &ind2)
{
    return ind1.fitness < ind2.fitness;
}

int main()
{
    srand((unsigned)time(NULL));

    int brGen = 0;
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

        vector<Individual> novaGeneracija;

        int udioStarih = (10 * VEL_POPULACIJE) / 100;
        for (int i = 0; i < udioStarih; i++)
        {
            novaGeneracija.push_back(populacija[i]);
        }

        int udioNovih = VEL_POPULACIJE - udioStarih;
        for (int i = 0; i < udioNovih; i++)
        {
            int r = rand() % (VEL_POPULACIJE / 2 + 1);
            Individual roditelj1 = populacija[r];

            r = rand() % (VEL_POPULACIJE / 2 + 1);
            Individual roditelj2 = populacija[r];

            novaGeneracija.push_back(roditelj1.mutacija(roditelj2));
        }
        populacija = novaGeneracija;
        cout << "Generacija " << brGen << endl;
        cout << "Najbolji primjerak: ";
        printGen(populacija[0]);
        cout << endl;
        cout << "Fitess najboljeg: " << populacija[0].fitness << endl
             << endl;

        brGen++;
    }
    cout << "Generacija " << brGen << endl;
    cout << "Najbolji primjerak: ";
    printGen(populacija[0]);
    cout << endl;
    cout << "Fitess najboljeg: " << populacija[0].fitness << endl;
}
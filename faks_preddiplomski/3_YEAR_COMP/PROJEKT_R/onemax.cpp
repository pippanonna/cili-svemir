#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#define VEL_POPULACIJE 100
#define VEL_GENOTIPA 20

#define PRECIZNOST 1000
#define DG_RODITELJ 0.45f
#define GG_RODITELJ 0.90f

const string GENI = "01";
string cilj = "";

char generirajGen()
{
    int len = GENI.size();
    int randBr = rand() % len;
    return GENI[randBr];
}

string generirajKromosom()
{
    string krom = "";
    for (int i = 0; i < VEL_GENOTIPA; i++)
    {
        krom += generirajGen();
    }
    return krom;
}

class Individual
{
public:
    string kromosom;
    int fitness;
    Individual(string kromosom);
    Individual mnoziSe(Individual roditelj2);
    int izracunaj_fitness();
};

Individual::Individual(string kromosom)
{
    this->kromosom = kromosom;
    fitness = izracunaj_fitness();
};

Individual Individual::mnoziSe(Individual roditelj2)
{
    string kromosom_djeteta = "";

    for (int i = 0; i < VEL_GENOTIPA; i++)
    {
        float p = (rand() % (PRECIZNOST + 1)) / PRECIZNOST;

        if (p < DG_RODITELJ)
        {
            kromosom_djeteta += this->kromosom[i];
        }
        else if (p < GG_RODITELJ)
        {
            kromosom_djeteta += roditelj2.kromosom[i];
        }
        else
        {
            kromosom_djeteta += generirajGen();
        }
    }

    return Individual(kromosom_djeteta);
};

int Individual::izracunaj_fitness()
{
    int fitness = 0;

    for (int i = 0; i < VEL_GENOTIPA; i++)
    {
        if (this->kromosom[i] != cilj[i])
            fitness++;
    }

    return fitness;
};

bool operator<(const Individual &ind1, const Individual &ind2)
{
    return ind1.fitness < ind2.fitness;
}

int main()
{
    srand((unsigned)(time(0)));

    int brGen = 0;
    vector<Individual> populacija;
    bool pronaden = 0;

    for (int i = 0; i < VEL_GENOTIPA; i++)
    {
        cilj += "1";
    }
    cout << cilj;

    for (int i = 0; i < VEL_POPULACIJE; i++)
    {
        populacija.push_back(Individual(generirajKromosom()));
    }

    while (!pronaden)
    {
        sort(populacija.begin(), populacija.end());

        if (populacija[0].fitness == 0)
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

            novaGeneracija.push_back(roditelj1.mnoziSe(roditelj2));
        }
        populacija = novaGeneracija;
        cout << "Generacija " << brGen << endl;
        cout << "Najbolji primjerak: " << populacija[0].kromosom << endl;
        cout << "Fitess najboljeg: " << populacija[0].fitness << endl
             << endl;

        brGen++;
    }
    cout << "Generacija " << brGen << endl;
    cout << "Najbolji primjerak: " << populacija[0].kromosom << endl;
    cout << "Fitess najboljeg: " << populacija[0].fitness << endl;
}
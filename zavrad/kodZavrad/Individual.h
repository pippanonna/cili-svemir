#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "NeuralNetwork.h"

class Individual {
public:
    NeuralNetwork *genotype;
    double fitness;
    int index;

    Individual(std::vector<int> hiddenLayers, int inputSize, int ind);
    Individual(int ind);
    ~Individual();
};


#endif

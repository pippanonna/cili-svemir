#ifndef EVOLUTIONARYALGORITHM_H
#define EVOLUTIONARYALGORITHM_H

#include "vector"
#include "Individual.h"
#include "Board.h"

class EvolutionaryAlgorithm {
public:
    int populationSize;
    int elitism;
    int iterations;
    int printIteration;
    double p; // mutation chance
    double K;
    int rows;
    int cols;
    std::vector<int> hiddenLayers;
    int inputSize;
    string fileName;
    std::vector<Individual *> population;

    EvolutionaryAlgorithm(int populationSize, int elitism, int iterations, int printIteration, double p, double K, int rows, int cols, std::vector<int> hiddenLayers, string result);
    ~EvolutionaryAlgorithm();

    void step();
    void initialize();
    void train();

    Individual* createIndividual(int ind);
    int playGame(Individual *player1, Individual *player2);
    void evaluate(Individual *player1, Individual *player2);
    double evaluateAgainstHeuristicOrRandom(Individual *player, bool useHeuristic);
    int heuristicMove(Eigen::MatrixXd board);
    int randomMove(Eigen::MatrixXd board);
};


#endif

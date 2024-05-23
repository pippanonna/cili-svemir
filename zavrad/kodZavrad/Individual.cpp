#include "Individual.h"
#include "random"
#include "iostream"
#include "chrono"

Individual::Individual(std::vector<int> hiddenLayers, int inputSize, int ind) {
    index = ind;
    fitness = 1e-5;

    int rows = inputSize * 2, cols = hiddenLayers.at(0);
    std::vector<Eigen::MatrixXd> matrices;

    std::default_random_engine generator;
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::normal_distribution<double> distribution(0, 0.01);

    for (unsigned long int i = 1; i < hiddenLayers.size() + 1; i++) {
        Eigen::MatrixXd m(rows + 1, cols + 1);

        for (int j = 0; j < rows; j++)
            m(j, cols) = 0;
        m(rows, cols) = 1;

        for (int j = 0; j <= rows; j++) {
            for (int k = 0; k < cols; k++) {
                m(j, k) = distribution(generator);
            }
        }

        matrices.push_back(m);

        if (i != hiddenLayers.size()) {
            rows = cols;
            cols = hiddenLayers.at(i);
        }
    }

    rows = cols;
    cols = 1;

    Eigen::MatrixXd m(rows + 1, cols + 1);

    for (int j = 0; j < rows; j++)
        m(j, cols) = 0;
    m(rows, cols) = 1;

    for (int j = 0; j <= rows; j++) {
        for (int k = 0; k < cols; k++) {
            m(j, k) = distribution(generator);
        }
    }

    matrices.push_back(m);

    genotype = new NeuralNetwork(matrices);
}

Individual::Individual(int ind) {
    index = ind;
    fitness = 1e-6;
}

Individual::~Individual() {
    delete this->genotype;
}
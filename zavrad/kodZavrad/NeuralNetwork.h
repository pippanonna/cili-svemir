#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "vector"
#include "iostream"
#include "C:\Eigen\Eigen\Dense"


class NeuralNetwork {
public:
    std::vector<Eigen::MatrixXd> matrices;

    NeuralNetwork(std::vector<Eigen::MatrixXd> m);
    ~NeuralNetwork();

    int feedForward(Eigen::MatrixXd v);
};


#endif

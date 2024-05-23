#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(std::vector<Eigen::MatrixXd> m) {
    matrices = m;
    for (auto v : matrices) {
        // std::cout << "MAT\n" <<  v << std::endl;
    }
}

NeuralNetwork::~NeuralNetwork() {}

double sigm(double x) {
    return 1 / (1 + exp(-x));
}

int NeuralNetwork::feedForward(Eigen::MatrixXd input) {
    int move = -1;
    double maxValue = 0;
    bool first = true;
    for (int i = 0; i < input.cols(); i++) {
        if (input(0, i) == 1.0) continue;
        Eigen::MatrixXd oneHot = Eigen::MatrixXd::Zero(input.rows(), input.cols());
        oneHot(0, i) = 1.0;


        Eigen::MatrixXd v(input.rows(), input.cols() + oneHot.cols() + 1);
        v << input, oneHot, 1.0;
        // std::cout << "Input: " << v << std::endl;
        for (unsigned long int j = 0; j < matrices.size(); j++) {
        //std::cout << "Matrix " << i << " size: " << matrices.at(i).rows() << "x" << matrices.at(i).cols() << std::endl;

            v = v * matrices.at(j);
            if (i != matrices.size() - 1) {
                v = v.unaryExpr(&sigm);
                v(0, v.cols() - 1) = 1;
            }
        }
        // std::cout << "Move " << i << ": " << v(0, 0) << std::endl;
        if (first) {
            first = false;
            maxValue = v(0, 0);
            move = i;
        } else if (v(0, 0) > maxValue) {
            maxValue = v(0, 0);
            move = i;
        }

    }
    return move;
}
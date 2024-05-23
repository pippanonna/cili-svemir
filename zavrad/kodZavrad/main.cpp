#include <iostream>
#include "C:\Eigen\Eigen\Dense"
#include "string"
#include "vector"
#include "sstream"
#include "fstream"
#include "iomanip"

#include "EvolutionaryAlgorithm.h"


std::string findArgument(std::string argName, int argc, char **argv);

int main(int argc, char* argv[]){
    int popSize, elitism, noOfIterations, rows, cols, printIteration;
    double p, K;
    string nnArchitecture, nnLayer, file;
    std::vector<int> hiddenLayers;

    popSize = stoi(findArgument("--popsize", argc, argv));
    elitism = stoi(findArgument("--elitism", argc, argv));
    noOfIterations = stoi(findArgument("--iter", argc, argv));
    printIteration = stoi(findArgument("--print", argc, argv));
    p = stod(findArgument("--p", argc, argv));
    K = stod(findArgument("--K", argc, argv));
    rows = stoi(findArgument("--rows", argc, argv));
    cols = stoi(findArgument("--cols", argc, argv));
    nnArchitecture = findArgument("--nn", argc, argv);
    file = findArgument("--file", argc, argv);

    stringstream ss(nnArchitecture);
    while(getline(ss, nnLayer, 's')) {
        hiddenLayers.push_back(stoi(nnLayer));
    }


    srand(time(NULL));
    std::cout << std::fixed;
    std::cout << std::setprecision(6);

    for (int i = 0; i < 5; i++) {
        EvolutionaryAlgorithm* ea = new EvolutionaryAlgorithm(popSize, elitism, noOfIterations, printIteration, p, K, rows, cols, hiddenLayers, file);
        ea->initialize();
        ea->train();
        delete ea;

    }
}

std::string findArgument(std::string argName, int argc, char **argv) {
    int i = 0;
    for (i = 0; i < argc; i++) {
        if (!argName.compare(argv[i])) {
            break;
        }
    }

    if (i == argc) {
        return "error";
    } 

    return argv[i + 1];
}

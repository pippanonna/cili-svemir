#include "EvolutionaryAlgorithm.h"
#include "iostream"
#include "chrono"
#include "random"
#include <fstream>

EvolutionaryAlgorithm::EvolutionaryAlgorithm(int populationSize, int elitism, int iterations, int printIteration, double p, double K, int rows, int cols, std::vector<int> hiddenLayers, string result) {
    this->populationSize = populationSize;
    this->rows = rows;
    this->cols = cols;
    this->elitism = elitism;
    this->iterations = iterations;
    this->printIteration = printIteration;
    this->p = p;
    this->K = K;
    this->hiddenLayers = hiddenLayers;
    inputSize = 2 * rows * cols + rows + cols;
    this->fileName = result;
}

EvolutionaryAlgorithm::~EvolutionaryAlgorithm() {
    for (int i = 0; i < population.size(); i++) {
        delete population.at(i);
    }
}

void EvolutionaryAlgorithm::initialize() {
    for (int i = 0; i < populationSize; i++) {
        // std::cout << "MAKING INDIVIDUAL " << i + 1 << std::endl;
        population.push_back(new Individual(hiddenLayers, inputSize, i + 1));
    }
    this->step();
}

Individual *EvolutionaryAlgorithm::createIndividual(int ind) {
    double f = 0;
    for (auto x : population) {
        f += x->fitness;
        // std::cout << x->fitness << endl;

    }
    // std::cout << "fitness " << f << endl;
    

    std::vector<Individual*> crossover_parents;
    double r, acc;
    int index;


    for (int i = 0; i < 2; i++) {

        r = ((double) rand() / RAND_MAX) * f;
        acc = 0;
        index = -1;
        while (acc < r) {
            index++;
            acc += population.at(index)->fitness;
        }
        crossover_parents.push_back(population.at(index));
    }


    Individual* u = new Individual(ind);
    std::vector<Eigen::MatrixXd> matrices;

    for (unsigned long int j = 0; j < crossover_parents.at(0)->genotype->matrices.size(); j++)
        matrices.push_back((crossover_parents.at(0)->genotype->matrices.at(j) + crossover_parents.at(1)->genotype->matrices.at(j)) / 2);

    u->genotype = new NeuralNetwork(matrices);

    if ((double) rand() / RAND_MAX < p) {
        std::default_random_engine generator;
        generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
        std::normal_distribution<double> distribution(0, K);

        for (unsigned long k = 0; k < u->genotype->matrices.size(); k++)
            for (int i = 0; i < u->genotype->matrices.at(k).rows(); i++)
                for (int j = 0; j < u->genotype->matrices.at(k).cols() - 1; j++)
                    u->genotype->matrices.at(k)(i,j) += distribution(generator);
    }

    return u;
}

void EvolutionaryAlgorithm::step() {
    for (int i = 0; i < population.size(); i++) {
        population.at(i)->fitness = 1e-6;
    }
    for (int i = 0; i < population.size() - 1; i++) {
        for (int j = i + 1; j < population.size(); j++) {
            this->evaluate(population.at(i), population.at(j));
        }
    }
    /* for (int i = 0; i < population.size(); i++) {
        std::cout << population.at(i)->index << ". fitness: " << population.at(i)->fitness << endl;
    } */
}


void EvolutionaryAlgorithm::train() {
    std::ofstream myfile;
    myfile.open(fileName, std::ios_base::app);

    for (int i = 1; i <= this->iterations; i++) {

        /* for (auto x : population) {
            std::cout << x->index << ". fitness: " << x->fitness << endl;
        } */

        std::sort(population.begin(), population.end(), [](Individual* u1, Individual* u2) {
            if (u1->fitness != u2->fitness) return (u1->fitness > u2->fitness);
            else return (u1->index < u2->index);
        });
        if (i % this->printIteration == 0) {
            myfile << evaluateAgainstHeuristicOrRandom(population.at(0), true) << ";";
            // std::cout << "[Train error @" << i << "]: " << population.at(0)->fitness << std::endl;
        }

        if (i != iterations) {
            std::vector<Individual*> new_population;
            for (int j = 0; j < elitism; j++) {
                population.at(j)->index = j + 1;
                new_population.push_back((population.at(j)));

            }

            for (int j = elitism; j < populationSize; j++) {
                Individual* u = createIndividual(j + 1);
                new_population.push_back(u);
            }


            for (int j = elitism; j < populationSize; j++)
                delete population.at(j);

            population = new_population;
        }

        
        this->step();
    }
    myfile << std::endl;
    myfile.close();
}

int EvolutionaryAlgorithm::playGame(Individual *player1, Individual *player2) {
    Board board = Board(rows, cols);
    int playerScores[2] = {0, 0};
    int current = 1, edgeNum = 0;

    while (!board.gameOver())
    {
        int claimed = 0;
        if (current == 1) {
            edgeNum = player1->genotype->feedForward(board.field);
        } else {
            edgeNum = player2->genotype->feedForward(board.field);
        }
        claimed = board.claimEdgeNum(edgeNum);

        if (claimed > 0) {
            playerScores[current - 1] += claimed;
        } else {
            current = current % 2 + 1;
        }

    }
    int won = 0;

    if (playerScores[0] > playerScores[1]) {
        won = 1;
    } else if (playerScores[0] < playerScores[1]) {
        won = 2;
    } 

    // std::cout << "Player " << won << " won!" << endl;
    return won;
}

void EvolutionaryAlgorithm::evaluate(Individual *player1, Individual *player2) {
    int won = playGame(player1, player2);

    switch (won)
    {
    case 1:
        player1->fitness += 2.0;
        break;
    case 2:
        player2->fitness += 2.0;
        break;
    
    default:
        player1->fitness += 1.0;
        player2->fitness += 1.0;
        break;
    }

    won = playGame(player2, player1);

    switch (won)
    {
    case 1:
        player2->fitness += 2.0;
        break;
    case 2:
        player1->fitness += 2.0;
        break;
    
    default:
        player1->fitness += 1.0;
        player2->fitness += 1.0;
        break;
    }
}

double EvolutionaryAlgorithm::evaluateAgainstHeuristicOrRandom(Individual *player, bool useHeuristic) {
    int numTest = 10;
    int fitness = 0;
    bool switchSides = false;
    for (int i = 0; i < numTest; i++) {
        Board board = Board(rows, cols);
        int playerScores[2] = {0, 0};
        int current, edgeNum = 0;

        if (!switchSides) {
            current = 1;
        } else {
            current = 2;
        }

        while (!board.gameOver())
        {
            int claimed = 0;
            if (current == 1) {
                edgeNum = player->genotype->feedForward(board.field);
            } else {
                if (useHeuristic)
                    edgeNum = heuristicMove(board.field);
                else
                    edgeNum = randomMove(board.field);
            }
            claimed = board.claimEdgeNum(edgeNum);

            if (claimed > 0) {
                playerScores[current - 1] += claimed;
            } else {
                current = current % 2 + 1;
            }

        }
        int won = 0;


        if (playerScores[0] > playerScores[1]) {
            // std::cout << "Player won!" << endl;
            fitness += 2;
        } else if (playerScores[0] == playerScores[1]) {
            // std::cout << "Tie!" << endl;
            fitness += 1;
        } else {
            // std::cout << "Heuristic won!" << endl;

        }

        if (i == (numTest / 2 - 1)) {
            switchSides = true;
        }
    }

    return (1.0 * fitness) / (2 * numTest);

}

int EvolutionaryAlgorithm::heuristicMove(Eigen::MatrixXd board) {
    int move = -1;
    vector<int> bestMoves;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board(0, i * (2 * cols + 1) + j) + board(0, i * (2 * cols + 1) + cols + j) + board(0, i * (2 * cols + 1) + cols + 1 + j) + board(0, (i + 1) * (2 * cols + 1) + j) == 3.0) {
                if (board(0, i * (2 * cols + 1) + j) == 0) {
                    bestMoves.push_back(i * (2 * cols + 1) + j);
                } else if (board(0, i * (2 * cols + 1) + cols + j) == 0) {
                    bestMoves.push_back(i * (2 * cols + 1) + cols + j);
                } else if (board(0,  i * (2 * cols + 1) + cols + 1 + j) == 0) {
                    bestMoves.push_back( i * (2 * cols + 1) + cols + 1 + j);
                } else if (board(0, (i + 1) * (2 * cols + 1) + j) == 0) {
                    bestMoves.push_back((i + 1) * (2 * cols + 1) + j);
                }
            }
        }
    }
    if (bestMoves.size() == 1) {
        move = bestMoves.at(0);
    } else if (bestMoves.size() > 1) {
        move = bestMoves.at(rand() % bestMoves.size());
    } else {
        do {
            move = rand() % board.cols();
        } while (board(0, move) == 1.0);
    }

    return move;
}

int EvolutionaryAlgorithm::randomMove(Eigen::MatrixXd board) {
    int move = -1;
    do {
        move = rand() % board.cols();
    } while (board(0, move) == 1.0);

    return move;
}

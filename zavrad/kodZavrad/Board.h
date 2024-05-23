#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <unordered_map>
#include "C:\Eigen\Eigen\Dense"


using namespace std;

class Board {
public:
    int rows;
    int cols;
    int claimedBoxes;
    int totalBoxes;
    Eigen::MatrixXd field;
    Board(int rows, int cols);
    ~Board();
    int claimEdgeNum(int edgeNum);
    bool gameOver();
};

#endif
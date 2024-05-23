#include "Board.h"
#include <iostream>
#include <string>

using namespace std;

Board::Board(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    this->claimedBoxes = 0;
    this->totalBoxes = this->rows * this->cols;
    this->field = Eigen::MatrixXd::Zero(1, 2 * rows * cols + rows + cols);
}

Board::~Board() {
    
}



int Board::claimEdgeNum(int edgeNum) {
    int claimed = 0;
    field(0, edgeNum) = 1.0;
	// std::cout << edgeNum << " : " << field << std::endl;
    if (edgeNum % (2 * cols + 1) >= 0 && edgeNum % (2 * cols + 1) < cols) {
        // ili top ili bottom
        if (edgeNum < 2 * rows * cols + rows) {
            if (field(0, edgeNum + cols) == 1.0 && field(0, edgeNum + cols + 1) == 1.0 && field(0, edgeNum + 2 * cols + 1) == 1.0) {
                claimed++;
            }
        }
        if (edgeNum > cols) {
            if (field(0, edgeNum - cols) == 1.0 && field(0, edgeNum - cols - 1) == 1.0 && field(0, edgeNum - 2 * cols - 1) == 1.0) {
                claimed++;
            }
        }
    } else {
        if (edgeNum % (2 * cols + 1) < 2 * cols) {
            if (field(0, edgeNum - cols) == 1.0 && field(0, edgeNum + cols + 1) == 1.0 && field(0, edgeNum + 1) == 1.0) {
                claimed++;
            }
        }
        if (edgeNum % (2 * cols + 1) > cols) {
            if (field(0, edgeNum + cols) == 1.0 && field(0, edgeNum - cols - 1) == 1.0 && field(0, edgeNum - 1) == 1.0) {
                claimed++;
            }
        }
    }
	this->claimedBoxes += claimed;
	return claimed;
}


bool Board::gameOver() {
    return this->claimedBoxes == this->totalBoxes;
}




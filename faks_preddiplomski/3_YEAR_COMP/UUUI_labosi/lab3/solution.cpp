#include "decisiontree.h"
#include <iostream>

int main(int argc, char **argv) {
    if (argc < 3) return 0;

    int level = -1;
    if (argc > 3) level = stoi(argv[3]);

    ID3 model = ID3();
    model.fit(argv[1], level);
    string predictions = model.predict(argv[2]);
    
}
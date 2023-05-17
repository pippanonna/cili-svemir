#include "Resolution.h"
#include <iostream>

int main(int argc, char **argv) {
    if (argc <= 2) return 0;

    Resolution r = Resolution(argv[2]);

    string keyword = argv[1];

    if (!keyword.compare("resolution")) {
        r.resolution();
    } else if (!keyword.compare("cooking")) {
        r.cooking(argv[3]);
    } else {
        cout << "Wrong input!\n";
    }
}
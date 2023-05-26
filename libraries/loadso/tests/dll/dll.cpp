#include "dll.h"

#include <iostream>

int add(int x, int y) {
    std::cout << "Call add " << x << ", " << y << std::endl;
    return x + y;
}
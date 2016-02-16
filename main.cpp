//
//  main.cpp
//  MatrixTest
//
//  Created by Anthony Super on 2/6/16.
//  Copyright © 2016 Noided Media. All rights reserved.
//

#include <iostream>
#include "matrix.hpp"

using namespace NM;

int main(int argc, const char * argv[]) {
    Matrix m{
        {14, 5, -2},
        {-3, 5, 4},
        {6, 9, 3}
    };
    std::cout << (m * m).determinant() << std::endl;
    return 0;
}

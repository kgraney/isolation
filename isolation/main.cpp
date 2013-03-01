//
//  main.cpp
//  isolation
//
//  Created by Kevin Graney on 2/28/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#include <iostream>
#include "board.h"
#include "point.h"

int main(int argc, const char * argv[])
{
    std::cout << "Isolation Game - COMS W4701" << std::endl;
    std::cout << "Kevin M. Graney (kmg2165@columbia.edu)" << std::endl;
    std::cout << std::endl;
    
    Board b;
    for (auto p : b.PointPerimeter(Point(1,2))) {
        std::cout << "(" << p.x() << "," << p.y() << ")" << std::endl;
    }
    
    std::cout << b;
    
    return 0;
}


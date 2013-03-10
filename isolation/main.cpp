//
//  main.cpp
//  isolation
//
//  Created by Kevin Graney on 2/28/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#include <iostream>
#include "engine.h"
#include "board.h"

int main(int argc, const char * argv[])
{
    std::cout << "Isolation Game - COMS W4701" << std::endl;
    std::cout << "Kevin M. Graney (kmg2165@columbia.edu)" << std::endl;
    std::cout << std::endl;
    
    Engine eng(kPlayerX);
    eng.TakeTurn();
    
    return 0;
}


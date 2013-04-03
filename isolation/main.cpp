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

    char p;
    int time_limit;
    do {
        std::cout << "Which player will AI be? [x/o] ";
        std::cin >> p;
    } while(p != 'x' && p != 'o');

    std::cout << "What's the time limit? ";
    std::cin >> time_limit;

    Player player;
    if (p == 'x')
        player = kPlayerX;
    else
        player = kPlayerO;

    Engine eng(player, time_limit);
    eng.PlayGame(true);

    return 0;
}


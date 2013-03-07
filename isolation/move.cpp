//
//  move.cpp
//  isolation
//
//  Created by Kevin Graney on 3/6/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#include "move.h"
#include "board.h"

Move::Move(int delta_x, int delta_y, Player p)
: player_(p), pos_delta_(Point(delta_x, delta_y))
{
}

void Move::ApplyToBoard(Board* b)
{
    switch (player_) {
        case kPlayerX:
            b->ClosePoint(b->get_xloc());
            b->set_xloc(b->get_xloc() + pos_delta_);
        case kPlayerO:
            b->ClosePoint(b->get_oloc());
            b->set_oloc(b->get_oloc() + pos_delta_);
        default:
            throw std::invalid_argument("Bad player in ApplyToBoard");
    }
}

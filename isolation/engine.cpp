//
//  engine.cpp
//  isolation
//
//  Created by Kevin Graney on 3/3/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#include "engine.h"
#include "board.h"

Engine::Engine(Player me)
: me_(me), current_board_(new Board())
{
    if (me_ == kPlayerX)
        opponent_ = kPlayerO;
    else
        opponent_ = kPlayerX;
}

Engine::GameState Engine::FindGameState() const
{
    if (current_board_->IsTerminalBoard()) {
        return kFinished;
    } else if (current_board_->IsIsolatedBoard()) {
        return kEndGame;
    } else {
        return kMatchup;
    }
}

std::unique_ptr<MoveList> Engine::BoardMoves_(const Board& board, Player player) const
{
    Point start = GetPosition_(player);
    std::unique_ptr<MoveList> l(new MoveList());
    std::shared_ptr<PointList> pl = board.OpenPoints(start);
    for(auto &p : *pl) {
        l->push_back(Move(p - start, player));
    }
    return l;
}

Point Engine::GetPosition_(Player player) const
{
    switch (player) {
        case kPlayerX:
            return current_board_->get_xloc();
            break;
        case kPlayerO:
            return current_board_->get_oloc();
            break;
        default:
            throw std::invalid_argument("Bad player in GetPosition_");
    }
}

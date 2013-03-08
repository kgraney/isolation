//
//  engine.h
//  isolation
//
//  Created by Kevin Graney on 3/3/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#ifndef __isolation__engine__
#define __isolation__engine__

#include <iostream>
#include <list>

#include "move.h"

class Board;

class Engine
{
public:
    typedef enum {
        kSparseBoard,
        kMatchup,
        kEndGame,
        kFinished
    } GameState;
    
    Engine(Player me);
    
    const std::shared_ptr<Board> get_current_board() { return current_board_; }
    
    Point MyPosition() { return GetPosition_(me_); }
    Point OpponentsPosition() { return GetPosition_(opponent_); }
    
    std::unique_ptr<MoveList> MyMoves() {
        return BoardMoves_(*current_board_, me_);
    }
    
private:
    GameState FindGameState() const;
    
    Point GetPosition_(Player player) const;
    std::unique_ptr<MoveList> BoardMoves_(const Board& board, Player player) const;
    
    std::shared_ptr<Board> current_board_;
    
    Player me_;
    Player opponent_;
};

#endif /* defined(__isolation__engine__) */

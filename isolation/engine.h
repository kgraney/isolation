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

#include "types.h"
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
    
    void TakeTurn();
    
private:
    GameState FindGameState() const;
    
    
    size_t Utility_(BoardPtr board) const;
    
    std::shared_ptr<NodePtrList> Successors_(Player player, std::shared_ptr<Node> node) const;
    
    int AlphaBeta(std::shared_ptr<Node> node);
    int MaxValue(std::shared_ptr<Node> node, int alpha, int beta, int depth_counter);
    int MinValue(std::shared_ptr<Node> node, int alpha, int beta, int depth_counter);
    
    //// Data
    
    std::shared_ptr<Board> current_board_;
    std::shared_ptr<Node> current_node_;
    
    Player me_;
    Player opponent_;
};

#endif /* defined(__isolation__engine__) */

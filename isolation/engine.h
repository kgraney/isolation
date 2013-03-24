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

const double kPInf = std::numeric_limits<double>::infinity();
const double kNInf = -std::numeric_limits<double>::infinity();

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
    
    void PlayGame(bool autoplay);
    
private:
    void TakeAITurn_();
    void TakeMeatTurn_();
    void TakeRandomTurn_();
    
    GameState FindGameState() const;
    
    
    double Utility_(BoardPtr board) const;
    bool CompareNodeUtility_(const NodePtr& a, const NodePtr& b) const;
    
    std::shared_ptr<NodePtrVec> Successors_(Player player, std::shared_ptr<Node> node) const;
    
    NodePtr AlphaBeta(std::shared_ptr<Node> node, int depth);
    NodePtr MaxValue(std::shared_ptr<Node> node, double alpha, double beta, int depth_counter, bool first);
    NodePtr MinValue(std::shared_ptr<Node> node, double alpha, double beta, int depth_counter, bool first);

    bool CutoffSearch_(NodePtr node) const;
    
    //// Data
    
    std::shared_ptr<Board> current_board_;
    std::shared_ptr<Node> current_node_;
    
    Player me_;
    Player opponent_;
    
    Player active_;
    Player inactive_;
};

#endif /* defined(__isolation__engine__) */

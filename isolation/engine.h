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
#include <limits>
#include <chrono>

#include "types.h"
#include "move.h"

class Board;

const double kPInf = std::numeric_limits<double>::infinity();
const double kNInf = -std::numeric_limits<double>::infinity();

class Engine
{
public:
    Engine(Player me, int turn_limit);
    
    const std::shared_ptr<Board> get_current_board() { return current_board_; }
    
    void PlayGame(bool autoplay);
    
private:
    void TakeAITurn_();
    void TakeMeatTurn_();
    void TakeRandomTurn_();
    
    GameState FindGameState(BoardPtr board) const;
    
    
    double Utility_(BoardPtr board) const;
    bool CompareNodeUtility_(const NodePtr& a, const NodePtr& b) const;
    bool CompareNodeUtilityReversed_(const NodePtr& a, const NodePtr& b) const;
    
    std::shared_ptr<NodePtrVec> Successors_(Player player, std::shared_ptr<Node> node) const;
    
    NodePtr AlphaBeta(std::shared_ptr<Node> node, int depth) const;
    NodePtr MaxValue(std::shared_ptr<Node> node, double alpha, double beta, int depth_counter, bool first) const;
    NodePtr MinValue(std::shared_ptr<Node> node, double alpha, double beta, int depth_counter, bool first) const;

    bool CutoffSearch_(NodePtr node) const;
    
    //// Data
    
    GameState current_state_;
    std::shared_ptr<Board> current_board_;
    std::shared_ptr<Node> current_node_;
    
    Player me_;
    Player opponent_;
    
    Player active_;
    Player inactive_;

    std::chrono::time_point<std::chrono::system_clock> turn_start_;
    int turn_limit_; // max seconds a turn can take
};

#endif /* defined(__isolation__engine__) */


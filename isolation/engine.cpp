//
//  engine.cpp
//  isolation
//
//  Created by Kevin Graney on 3/3/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#include <time.h>
#include <functional>

#include "engine.h"
#include "board.h"
#include "node.h"

using namespace std::placeholders;

bool CompareNodeValues(const NodePtr& a, const NodePtr& b)
{
    return a->get_value() < b->get_value();
}

bool Engine::CompareNodeUtility_(const NodePtr& a, const NodePtr& b) const
{
    return Utility_(a->get_board()) < Utility_(b->get_board()); 
}

Engine::Engine(Player me)
: me_(me), current_board_(new Board()), current_node_(new Node(current_board_))
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



std::shared_ptr<NodePtrVec> Engine::Successors_(Player player, std::shared_ptr<Node> node) const
{
/*
    std::unique_ptr<MoveVec> moves = node->get_board()->Moves(player);

    std::shared_ptr<NodePtrVec> nodes(new NodePtrVec());
    for (auto move : *moves) {
        std::shared_ptr<Board> new_board(new Board(*(node->get_board())));
        move.ApplyToBoard(new_board);
        nodes->push_back(std::shared_ptr<Node>(new Node(new_board, node)));
    }
    return nodes;
*/
    return node->get_board()->Successors(player, node);
}

void Engine::TakeTurn()
{
    std::cout << "Entering board:" << std::endl;
    std::cout << *(get_current_board()) << std::endl;
    
    
    clock_t begin, end;
    double time_spent;

    
    for(int i=0; i < 1000; i++) {
        
        begin = clock();
        
        Player winner = current_board_->IsTerminalBoard();
        if (winner) {
            std::cout << "End of game " << winner << " wins." << std::endl;
            break;
        }
    
        BoardPtr new_board(new Board(*current_board_));
        NodePtr new_node(new Node(new_board));
        
        NodePtr best_node = AlphaBeta(new_node, 4);
        int best_value = best_node->get_value();
        
        if (!best_node) {
            Player winner = current_board_->IsTerminalBoard();
            std::cout << "End of game " << winner << " wins." << std::endl;
            break;
        }
        
        end = clock();
        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        
        std::cout << me_ << " board = " << i << " time = " << time_spent << " utility = " << best_value << std::endl;
        std::cout << *best_node->get_board() << std::endl;
        BoardPtr next_board = best_node->get_board();

        std::cout << "----------------- SWITCH TURNS -----------------" << std::endl;
        current_board_ = next_board;
        std::swap(opponent_, me_);
    }
}

NodePtr Engine::AlphaBeta(std::shared_ptr<Node> node, int depth)
{
    NodePtr best = MaxValue(node, INT32_MIN, INT32_MAX, depth);  // ENSURE EVEN/ODD DEPTH CORRECT!;
    NodePtr next = best;
    while (next->get_parent() != node)
        next = next->get_parent();
    return next;
}

NodePtr Engine::MaxValue(std::shared_ptr<Node> node, int alpha, int beta, int depth_counter)
{
    if (depth_counter == 0 || node->get_board()->IsTerminalBoard()) {
        node->set_value(Utility_(node->get_board()));
        return node;
    }

    NodePtr v = NodePtr(new Node(nullptr));
    v->set_value(INT32_MIN);
    std::shared_ptr<NodePtrVec> lst = Successors_(me_, node);
    //lst->sort([this] (NodePtr a, NodePtr b) {
    std::sort(lst->begin(), lst->end(), std::bind(&Engine::CompareNodeUtility_, this, _1, _2));

    for (auto child : *lst) {
        v = std::max(v, MinValue(child, alpha, beta, depth_counter - 1), &CompareNodeValues);
        if (v->get_value() >= beta) {
            return v;
        }
        alpha = std::max(alpha, v->get_value());
    }

    return v;
}

NodePtr Engine::MinValue(std::shared_ptr<Node> node, int alpha, int beta, int depth_counter)
{
    if (depth_counter == 0 || node->get_board()->IsTerminalBoard()) {
        node->set_value(Utility_(node->get_board()));
        return node;
    }
    
    NodePtr v = NodePtr(new Node(nullptr));
    v->set_value(INT32_MAX);

    std::shared_ptr<NodePtrVec> lst = Successors_(opponent_, node);
    //lst->sort([this] (NodePtr a, NodePtr b) {
    std::sort(lst->begin(), lst->end(), std::bind(&Engine::CompareNodeUtility_, this, _1, _2));

    for (auto child : *lst) {
        v = std::min(v, MaxValue(child, alpha, beta, depth_counter - 1), &CompareNodeValues);
        if (v->get_value() <= alpha) {
            return v;
        }
        beta = std::min(beta, v->get_value());
    }
    
    return v;
}


size_t Engine::Utility_(BoardPtr board) const
{
    //if (board->IsIsolatedBoard()) {
    //    return board->NumMoves(me_); // TODO: Enhance
    //} else {
        return 2*board->NumMoves(me_) - board->NumMoves(opponent_);
    //}
}

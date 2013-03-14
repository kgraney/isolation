//
//  engine.cpp
//  isolation
//
//  Created by Kevin Graney on 3/3/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#include <time.h>

#include "engine.h"
#include "board.h"
#include "node.h"

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



std::shared_ptr<NodePtrList> Engine::Successors_(Player player, std::shared_ptr<Node> node) const
{
    std::unique_ptr<MoveList> moves = node->get_board()->Moves(player);

    std::shared_ptr<NodePtrList> nodes(new NodePtrList);
    for (auto move : *moves) {
        std::shared_ptr<Board> new_board(new Board(*(node->get_board())));
        move.ApplyToBoard(new_board);
        nodes->push_back(std::shared_ptr<Node>(new Node(new_board, node)));
    }
    return nodes;
}

void Engine::TakeTurn()
{
    std::cout << "Entering board:" << std::endl;
    std::cout << *(get_current_board()) << std::endl;
    
    
    clock_t begin, end;
    double time_spent;

    
    for(int i=0; i < 100; i++) {
        
        begin = clock();
        
        Player winner = current_board_->IsTerminalBoard();
        if (winner) {
            std::cout << "End of game " << winner << " wins." << std::endl;
            break;
        }
        
        std::shared_ptr<NodePtrList> lst = Successors_(me_, NodePtr(new Node(current_board_)));
        lst->sort([this] (NodePtr a, NodePtr b) {
            return this->Utility_(a->get_board()) > this->Utility_(b->get_board());
        });
        
        NodePtr best_node = lst->front();
        int best_value = INT32_MIN;
        for (auto node : *lst) {
            //std::cout << *node->get_board() << std::endl;
            int value = AlphaBeta(node);
            if (value > best_value) {
                best_node = node;
                best_value = value;
            }
        }
        
        if (!best_node) {
            Player winner = current_board_->IsTerminalBoard();
            std::cout << "End of game " << winner << " wins." << std::endl;
            break;
        }
        
        end = clock();
        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        
        std::cout << me_ << " board = " << i << " time = " << time_spent << " utility = " << best_value << std::endl;
        std::cout << *best_node->get_board() << std::endl;
         
        current_board_ = best_node->get_board();
        std::swap(opponent_, me_);
    }
}

int Engine::AlphaBeta(std::shared_ptr<Node> node)
{
    int v = MaxValue(node, INT32_MIN, INT32_MAX, 3);  // ENSURE EVEN/ODD DEPTH CORRECT!
    //std::cout << v << std::endl;
    //std::cout << *node->get_board();
    return v; // TODO
}

int Engine::MaxValue(std::shared_ptr<Node> node, int alpha, int beta, int depth_counter)
{
    //if (node->get_board()->IsTerminalBoard())
    if (depth_counter == 0)
        return Utility_(node->get_board());
    
    int v = INT32_MIN;
    std::shared_ptr<NodePtrList> lst = Successors_(me_, node);
    lst->sort([this] (NodePtr a, NodePtr b) {
        return this->Utility_(a->get_board()) > this->Utility_(b->get_board());
    });

    for (auto child : *lst) {
        //std::cout << "MAX v=" << v << " a=" << alpha << " b=" << beta << std::endl;
        v = std::max(v, MinValue(child, alpha, beta, depth_counter - 1));
        if (v >= beta) {
            node->set_value(v);
            return v;
        }
        alpha = std::max(alpha, v);
    }
    node->set_value(v);
    return v;
}

int Engine::MinValue(std::shared_ptr<Node> node, int alpha, int beta, int depth_counter)
{
    //if (node->get_board()->IsTerminalBoard())
    if (depth_counter == 0)
        return Utility_(node->get_board());
    
    int v = INT32_MAX;
    std::shared_ptr<NodePtrList> lst = Successors_(opponent_, node);
    lst->sort([this] (NodePtr a, NodePtr b) {
        return this->Utility_(a->get_board()) < this->Utility_(b->get_board());
    });

    for (auto child : *lst) {
        v = std::min(v, MaxValue(child, alpha, beta, depth_counter - 1));
        if (v <= alpha) {
            node->set_value(v);
            return v;
        }
        beta = std::min(beta, v);
    }
    node->set_value(v);
    return v;
}


size_t Engine::Utility_(BoardPtr board) const
{
    return board->NumMoves(me_) - board->NumMoves(opponent_);
    //return board->Moves(me_)->size() - board->Moves(opponent_)->size();
}

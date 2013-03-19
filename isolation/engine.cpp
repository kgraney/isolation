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
    std::shared_ptr<MovePtrVec> moves = node->get_board()->SuccessorMoves(player);
    std::shared_ptr<NodePtrVec> nodes(new NodePtrVec());
    for (auto move: *moves) {
        BoardPtr new_board(new Board(*current_board_));
        move->ApplyToBoard(new_board);
     
        NodePtr new_node(new Node(new_board, node));
        nodes->push_back(new_node);
    }
    return nodes;
}

void Engine::PlayGame(bool autoplay)
{
    
    clock_t begin, end;
    double time_spent;
    active_ = me_;
    inactive_ = opponent_;

    srand(time(NULL));
    
    for(int i=0; i < 1000; i++) {
        
        // Check if someone has won yet
        Player winner = current_board_->IsTerminalBoard();
        if (winner) {
            std::cout << "################# !! END GAME !! #################" << std::endl;
            std::cout << "--------------    " << winner << " wins!    --------------" << std::endl;
            std::cout << "turn #: " << i << " last turn time: " << time_spent << std::endl;
            std::cout << std::endl;
            std::cout << *current_board_;
            std::cout << std::endl;
            break;
        }
        
        std::cout << "----------------- ** NEW TURN ** -----------------" << std::endl;
        std::cout << "-----------------    " << active_ << "    -----------------" << std::endl;
        std::cout << "turn #: " << i << " last turn time: " << time_spent << std::endl;
        std::cout << std::endl;
        std::cout << *current_board_;
        std::cout << std::endl;
        
        Point point = current_board_->GetPosition(active_);
        std::cout << "Possible moves from " << point << ":" << std::endl;
        auto pv = *current_board_->OpenPoints(point);
        Board b;
        b.clear();
        b.SetPosition(active_, current_board_->GetPosition(active_));
        for (auto p : pv) {
            b.ClosePoint(p);
        }
        std::cout << b << std::endl;
        
        begin = clock();

        if(active_ == me_)
            TakeAITurn_();
        else
            TakeRandomTurn_();

        end = clock();
        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        
        std::cout << std::endl;
        
        std::swap(active_, inactive_);
    }
}

void Engine::TakeAITurn_()
{

    BoardPtr new_board(new Board(*current_board_));
    NodePtr new_node(new Node(new_board));
    
    NodePtr best_node = AlphaBeta(new_node, 5);
    int best_value = best_node->get_value();
    std::cout << "AI: move with utility " << best_value << std::endl;
    
    if (!best_node) {
        Player winner = current_board_->IsTerminalBoard();
        std::cout << "!!! End of game " << winner << " wins." << std::endl;
    }
    
    current_board_ = best_node->get_board();;
}

void Engine::TakeRandomTurn_()
{
    auto pv = *current_board_->OpenPoints(current_board_->GetPosition(active_));
    int rand_index = rand() % pv.size();
    Move move(pv[rand_index] - current_board_->GetPosition(active_), active_);
    move.ApplyToBoard(current_board_);
}

void Engine::TakeMeatTurn_()
{
    Point new_loc;
    std::cout << "Enter coordinates " << active_ << " will move to." << std::endl;
    std::cin >> new_loc;
    
    // TODO: validate move
    Move move(new_loc - current_board_->GetPosition(active_), active_);
    move.ApplyToBoard(current_board_);
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
    std::shared_ptr<NodePtrVec> lst = Successors_(active_, node);
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

    std::shared_ptr<NodePtrVec> lst = Successors_(inactive_, node);
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
        return 2*board->NumMoves(active_) - board->NumMoves(inactive_);
    //}
}

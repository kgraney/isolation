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
    return Utility_(a->get_board()) > Utility_(b->get_board()); 
}

Engine::Engine(Player me)
: me_(me), current_board_(new Board()), current_node_(new Node(current_board_))
{
    if (me_ == kPlayerX)
        opponent_ = kPlayerO;
    else
        opponent_ = kPlayerX;
}

GameState Engine::FindGameState(BoardPtr board) const
{
    if (board->IsTerminalBoard()) {
        return kFinished;
    } else if (board->IsIsolatedBoard()) {
        return kIsolated;
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
    active_ = kPlayerX;
    inactive_ = kPlayerO;

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
        if (current_board_->IsIsolatedBoard()) {
            std::cout << "Isolated board state." << std::endl;
            int x_reach = current_board_->NumReachable(kPlayerX);
            int o_reach = current_board_->NumReachable(kPlayerO);
            if (x_reach > o_reach) {
                std::cout << "X likely to win" << std::endl;
            } else if (o_reach > x_reach) {
                std::cout << "O likely to win" << std::endl;
            } else {
                std::cout << "Too close to call" << std::endl;
            }
        }
        
/*
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
*/
        
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
    NodePtr best_node;
    for (int i=2; i <= 6; i+=2) {
        BoardPtr new_board(new Board(*current_board_));
        NodePtr new_node(new Node(new_board));
        clock_t start = clock();
    
        best_node = AlphaBeta(new_node, i);
        double time_spent = (double)(clock() - start) / CLOCKS_PER_SEC;
        std::cout << "depth = " << i << " time = " << time_spent;
        std::cout << " utility = " << best_node->get_value() << std::endl;
        //std::cout << *best_node->get_board();
    }

    if (!best_node) {
        Player winner = current_board_->IsTerminalBoard();
        std::cout << "!!! End of game " << winner << " wins." << std::endl;
    }

    current_board_ = best_node->get_board();;
}

void Engine::TakeRandomTurn_()
{
    auto pv = *current_board_->OpenPoints(current_board_->GetPosition(active_));

    int rand_index = rand() % (int)pv.size();
    std::cout << "Choose: " << pv[rand_index] << std::endl;

    Move move(pv[rand_index] - current_board_->GetPosition(active_), active_);
    move.ApplyToBoard(current_board_);
}

void Engine::TakeMeatTurn_()
{
    Point new_loc;
    std::cout << "Enter coordinates " << active_ << " will move to." << std::endl;
prompt_coord:
    std::cin >> new_loc;
    
    auto pv = *current_board_->OpenPoints(current_board_->GetPosition(active_));
    if (std::find(pv.begin(), pv.end(), new_loc) != pv.end())
        std::cout << "Valid move!" << std::endl;
    else {
        std::cout << "Invalid move, try again!" << std::endl;
        goto prompt_coord;
    }

    Move move(new_loc - current_board_->GetPosition(active_), active_);
    move.ApplyToBoard(current_board_);
}

NodePtr Engine::AlphaBeta(std::shared_ptr<Node> node, int depth)
{
    NodePtr best = MaxValue(node, kNInf, kPInf, depth, true);  // ENSURE EVEN/ODD DEPTH CORRECT!;
    NodePtr next = best;
    while (next->get_parent() != node)
        next = next->get_parent();

    next->set_value(best->get_value());
    return next;
}

NodePtr Engine::MaxValue(std::shared_ptr<Node> node, double alpha, double beta, int depth_counter, bool first)
{
    if (depth_counter == 0 || (CutoffSearch_(node) && !first)) {
        node->set_value(Utility_(node->get_board()));
        return node;
    }

    NodePtr v = NodePtr(new Node(nullptr));
    v->set_value(kNInf);
    std::shared_ptr<NodePtrVec> lst = Successors_(active_, node);
    std::sort(lst->begin(), lst->end(), std::bind(&Engine::CompareNodeUtility_, this, _1, _2));

    for (auto child : *lst) {
        v = std::max(v, MinValue(child, alpha, beta, depth_counter - 1, false), &CompareNodeValues);
        if (v->get_value() >= beta) {
            return v;
        }
        alpha = std::max(alpha, v->get_value());
    }

    return v;
}

NodePtr Engine::MinValue(std::shared_ptr<Node> node, double alpha, double beta, int depth_counter, bool first)
{
    if (depth_counter == 0 || (CutoffSearch_(node) && !first)) {
        node->set_value(Utility_(node->get_board()));
        return node;
    }
    
    NodePtr v = NodePtr(new Node(nullptr));
    v->set_value(kPInf);

    std::shared_ptr<NodePtrVec> lst = Successors_(inactive_, node);
    std::sort(lst->begin(), lst->end(), std::bind(&Engine::CompareNodeUtility_, this, _1, _2));

    for (auto child : *lst) {
        v = std::min(v, MaxValue(child, alpha, beta, depth_counter - 1, false), &CompareNodeValues);
        if (v->get_value() <= alpha) {
            return v;
        }
        beta = std::min(beta, v->get_value());
    }
    
    return v;
}

bool Engine::CutoffSearch_(NodePtr node) const
{
    return node->get_board()->IsTerminalBoard() || node->get_board()->IsIsolatedBoard();
}

double Engine::Utility_(BoardPtr board) const
{
    if (board->IsIsolatedBoard()) {
        // TODO: Enhance
        int active_reach = board->NumReachable(active_);
        int inactive_reach = board->NumReachable(inactive_);
        if (active_reach > inactive_reach)
            return kPInf;
            //return active_reach - inactive_reach + 1000;
        else
            return kNInf;
    } else {
        return board->NumMoves(active_) - board->NumMoves(inactive_);
    }
}

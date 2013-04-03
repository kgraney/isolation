//
//  engine.cpp
//  isolation
//
//  Created by Kevin Graney on 3/3/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#include <time.h>
#include <functional>
#include <algorithm>
#include <future>

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

bool Engine::CompareNodeUtilityReversed_(const NodePtr& a, const NodePtr& b) const
{
    return Utility_(a->get_board()) > Utility_(b->get_board()); 
}

Engine::Engine(Player me, double turn_limit, int num_threads)
: me_(me), current_board_(new Board()), current_node_(new Node(current_board_)),
    turn_limit_(turn_limit), num_threads_(num_threads)
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
    
    int i = 0;
    while (true) {
        i++;
       
        std::cout << "----------------- ** NEW TURN ** -----------------" << std::endl;
        std::cout << "-----------------    " << active_ << "    -----------------" << std::endl;
        std::cout << "turn #: " << i << " last turn time: " << time_spent << std::endl;
        std::cout << std::endl;
        std::cout << *current_board_;
        std::cout << std::endl;

        //GameState current_state_ = FindGameState(current_board_);
        //std::cout << "Game state: " << current_state_ << std::endl;
        std::cout << "starting move..." << std::endl;
        begin = clock();
        turn_start_ = std::chrono::system_clock::now();


        if (current_board_->IsIsolatedBoard()) {
            int x_reach = current_board_->NumReachable(kPlayerX);
            int o_reach = current_board_->NumReachable(kPlayerO);
            if (x_reach > o_reach) {
                std::cout << "X likely to win" << std::endl;
            } else if (x_reach < o_reach) {
                std::cout << "O likely to win" << std::endl;
            } else {
                std::cout << "Too close to call" << std::endl;
            }
        }
 
        // Check if someone has won yet
        Player winner = current_board_->IsTerminalBoard();
        if (winner) {
            if (winner == kAllPlayers)
                winner = active_; // next player loses if this is a terminal board
            std::cout << "################# !! END GAME !! #################" << std::endl;
            std::cout << "--------------    " << winner << " wins!    --------------" << std::endl;
            std::cout << "turn #: " << i << " last turn time: " << time_spent << std::endl;
            std::cout << std::endl;
            std::cout << *current_board_;
            std::cout << std::endl;
            break;
        }
 
        
        if(active_ == me_)
            TakeAITurn_();
        else
            TakeRandomTurn_();

        end = clock();
        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        
        std::cout << std::endl;
        std::cout << active_ << " moved to " << current_board_->GetPosition(active_) << std::endl;
       
        std::swap(active_, inactive_);
    }
}

void Engine::TakeAITurn_()
{
    NodePtr best_node;

    std::future<NodePtr> *threads = new std::future<NodePtr> [num_threads_];

    for (int i=0; i < num_threads_; i++) {
        BoardPtr new_board(new Board(*current_board_));
        NodePtr new_node(new Node(new_board));
        threads[i] = std::async(std::launch::async, &Engine::AlphaBeta, this, new_node, 2*i+2);
    }

    for (int i=0; i <= 16; i++) {
        clock_t start = clock();

        NodePtr result = threads[i%num_threads_].get();

        double time_spent = (double)(clock() - start) / CLOCKS_PER_SEC;
        std::cout << "depth = " << 2*i+2 << " time = " << time_spent;
        std::cout << " utility = " << result->get_value() << std::endl;
        if (!(result->time_expired_)) {
            best_node = result;
        } else {
            std::cout << "time expired, pre-mature cut-off" << std::endl;
            break;
        }

        BoardPtr new_board(new Board(*current_board_));
        NodePtr new_node(new Node(new_board));
        threads[i%num_threads_] = std::async(std::launch::async,
                &Engine::AlphaBeta, this, new_node, 2*(i+num_threads_)+2);
    }

    if (!best_node) {
        Player winner = current_board_->IsTerminalBoard();
        std::cout << "!!! End of game " << winner << " wins." << std::endl;
    }

    delete[] threads;
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

NodePtr Engine::AlphaBeta(std::shared_ptr<Node> node, int depth) const
{
    //std::cout << "Searching depth " << depth << " in thread " << std::this_thread::get_id() << std::endl;
    NodePtr best = MaxValue(node, kNInf, kPInf, depth, true);  // ENSURE EVEN/ODD DEPTH CORRECT!;
    NodePtr next = best;
    while (next->get_parent() != node)
        next = next->get_parent();

    next->set_value(best->get_value());
    //std::cout << "Finished depth " << depth << std::endl;
    return next;
}

NodePtr Engine::MaxValue(std::shared_ptr<Node> node, double alpha, double beta, int depth_counter, bool first) const
{
    if (depth_counter == 0 || (CutoffSearch_(node) && !first)) {
        node->set_value(Utility_(node->get_board()));
        return node;
    }

    std::shared_ptr<NodePtrVec> lst = Successors_(active_, node);
    std::sort(lst->begin(), lst->end(), std::bind(&Engine::CompareNodeUtilityReversed_, this, _1, _2));

    NodePtr v = *lst->begin();
    v->set_value(kNInf);

    for (auto child : *lst) {
        v = std::max(v, MinValue(child, alpha, beta, depth_counter - 1, false), &CompareNodeValues);
        if (v->get_value() >= beta) {
            return v;
        }
        alpha = std::max(alpha, v->get_value());
    }

    return v;
}

NodePtr Engine::MinValue(std::shared_ptr<Node> node, double alpha, double beta, int depth_counter, bool first) const
{
    if (depth_counter == 0 || (CutoffSearch_(node) && !first)) {
        node->set_value(Utility_(node->get_board()));
        return node;
    }
    
    std::shared_ptr<NodePtrVec> lst = Successors_(inactive_, node);
    std::sort(lst->begin(), lst->end(), std::bind(&Engine::CompareNodeUtility_, this, _1, _2));

    NodePtr v = *lst->begin();
    v->set_value(kPInf);

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
    auto now_time = std::chrono::system_clock::now();
    int total_seconds = std::chrono::duration_cast<std::chrono::milliseconds> (now_time-turn_start_).count();
    bool time_expired = total_seconds >= (1000*turn_limit_);
    node->time_expired_ = time_expired;

    return time_expired || node->get_board()->IsTerminalBoard() || node->get_board()->IsIsolatedBoard();
}

double Engine::Utility_(BoardPtr board) const
{
    GameState state = FindGameState(board);
    int active_reach, inactive_reach;
    Player winner;

    switch(state) {
        // Winning boards are as great as possible, losing boards as bad as possible.
        case kFinished:
            winner = board->IsTerminalBoard();
            if (winner == active_) return kPInf;
            else return kNInf;
            break;


        case kIsolated:
            active_reach = board->NumReachable(active_);
            inactive_reach = board->NumReachable(inactive_);
            if (active_reach > inactive_reach)
                return active_reach - inactive_reach + 10000;
            else
                return kNInf;

            break;

        case kMatchup:
        default:
            if (active_ == me_)
                return 2*board->NumMoves(active_) - board->NumMoves(inactive_);
            else
                return board->NumMoves(active_) - board->NumMoves(inactive_);
            break;
    }
}


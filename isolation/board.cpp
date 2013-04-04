//
//  board.cpp
//  isolation
//
//  Created by Kevin Graney on 2/28/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#include <exception>
#include <stdexcept>

#include "board.h"
#include "point.h"
#include "move.h"
#include "node.h"

const std::initializer_list<Point> Board::kPointDirections = {
    Point(0,1), Point(1,0),
    Point(-1,0), Point(0,-1),
    Point(-1,1), Point(1,-1),
    Point(1,1), Point(-1,-1)
};

Board::Board()
: xloc_(0,0), oloc_(kSize-1,kSize-1)
{
    for (int x=0; x < kSize; x++) {
        for (int y=0; y < kSize; y++) {
            array_[x][y] = true;
        }
    }
    
    // Close the two starting points, which are directly initialized, and therefore not
    // closed by set_loc_
    ClosePoint(Point(0,0));
    ClosePoint(Point(kSize-1, kSize-1));
}

Board::Board(const Board& board)
: xloc_(board.xloc_), oloc_(board.oloc_)
{
    for (int x=0; x < kSize; x++)
        for (int y=0; y < kSize; y++)
            array_[x][y] = board.array_[x][y];
}

void Board::clear()
{
    for (int x=0; x < kSize; x++)
        for (int y=0; y < kSize; y++)
            array_[x][y] = true;
    xloc_ = Point(-1,-1);
    oloc_ = Point(-1,-1);
}

void Board::set_loc_(const Point& pt, Point* loc)
{
    if (OnBoard(pt)) {
        ClosePoint(pt);
        *loc = pt;
    } else
        throw std::out_of_range("Point not on the board");
}

bool Board::PointOpen(const Point& pt) const
{
    if (OnBoard(pt))
        return array_[pt.x()][pt.y()];
    else
        //throw std::out_of_range("Point not on the board");
        return false;
}

bool Board::PointLooseOpen(const Point& pt) const
{
    return pt == get_xloc() || pt == get_oloc() || PointOpen(pt);
}

void Board::ClosePoint(const Point& pt)
{
    if (OnBoard(pt)) {
        array_[pt.x()][pt.y()] = false;
    } else
        throw std::out_of_range("Point not on the board");
}

bool Board::InvalidDiagonal(const Point& ref, const Point& dir) const
{
    return false; // All diagonal moves are legal?

    if (dir.x() == 0 || dir.y() == 0)
        return false; // not a diagonal move

    if (PointOpen(ref - Point(dir.x(), 0)) || PointOpen(ref - Point(0, dir.y())))
        return false;

    return true;
}

std::shared_ptr<PointVec> Board::OpenPoints(const Point& ref) const
{
    std::shared_ptr<PointVec> l(new PointVec);
    OpenPointSearch_(ref, Point(0,0), l);
    return l;
}

size_t Board::OpenPointSearch_(const Point& ref, Point direction, std::shared_ptr<PointVec> lst) const
{
    if (!OnBoard(ref))
        return 0;
        
    if (direction != Point(0,0)) {
        // for subsequent movements, continue on in the same direction

        if (PointOpen(ref)) {
            if (InvalidDiagonal(ref, direction))
                return 0;

            if (lst)
                lst->push_back(ref);
            return 1 + OpenPointSearch_(ref + direction, direction, lst);
        } else
            return 0;

    } else {
        // for the first movement, try each direction
        size_t r = 0;
        for (auto dir : kPointDirections) {
            r += OpenPointSearch_(ref + dir, dir, lst);
        }
        return r;
    }
}

bool Board::IsPointBlocked(const Point& ref, Point direction) const
{
    if (!OnBoard(ref))
        return true;
    
    if (direction != Point(0,0)) {
        if (InvalidDiagonal(ref, direction))
            return true;

        if (PointOpen(ref))
            return false;
        else
            return true;
    } else {
        bool blocked = true;
        for (auto dir : kPointDirections) {
            blocked = blocked && IsPointBlocked(ref + dir, dir);
        }
        return blocked;
    }
}

std::ostream& operator<< (std::ostream& stream, const Board& board)
{
    stream << "  ";
    for (int x=0; x < Board::kSize; x++)
        stream << 1+x << " ";
    stream << std::endl;
    
    for (int y=0; y < Board::kSize; y++) {
        stream << 1+y << " ";
        for (int x=0; x < Board::kSize; x++) {
            Point pt(x,y);
            
            if (pt == board.get_xloc())
                stream << 'x';
            else if(pt == board.get_oloc())
                stream << 'o';
            else
                stream << (board.PointOpen(pt) ? '-' : '*');
            stream << ' ';
        }
        stream << std::endl;
    }
    return stream;
}

Player Board::IsTerminalBoard()
{
    bool x_blocked = IsPointBlocked(get_xloc());
    bool o_blocked = IsPointBlocked(get_oloc());
    
    if (x_blocked && o_blocked) {
        //throw std::runtime_error("Tie game?");
        return kAllPlayers;
    } else if (x_blocked) {
        return kPlayerO;
    } else if (o_blocked) {
        return kPlayerX;
    } else {
        return kNoPlayer;
    }
}

std::unique_ptr<PointList> Board::PointPerimeter(const Point p) const
{
    // TODO: handle diagonal case properly
    std::unique_ptr<PointList> l(new PointList);
    for (auto dir : kPointDirections) {
        Point candidate = p + dir;
        if (InvalidDiagonal(candidate, dir))
            continue;

        if (OnBoard(candidate) && PointLooseOpen(candidate))
            l->push_back(candidate);
    }
    return l;
}

bool Board::OnBoard(const Point& p) const
{
    return p.x() >= 0 && p.y() >= 0 && p.x() < kSize && p.y() < kSize;
}

bool Board::IsIsolatedBoard()
{
    PointSet* explored = new PointSet;
    bool result = SearchForPath_(get_xloc(), get_oloc(), explored);
    delete explored;

    return !result;
}

bool Board::SearchForPath_(const Point& start, const Point& goal, PointSet* explored) const
{
    explored->insert(start);
    if (start == goal)
        return true;
    
    // find the possible one-step moves, and sort according to distance of that
    // point from the goal (closest first)
    std::unique_ptr<PointList> lst = PointPerimeter(start);
    lst->sort([&goal] (const Point& p1, const Point& p2) {
        return p1.DistanceTo(goal) < p2.DistanceTo(goal);
    });

    bool b = false;
    for (auto p : *lst) {
        if (explored->find(p) == explored->end()) {
            b = b || SearchForPath_(p, goal, explored);
            if(b) break; // short-circuit or
        }
    }
    return b;
}

Point Board::GetPosition(Player player) const
{
    switch (player) {
        case kPlayerX:
            return get_xloc();
            break;
        case kPlayerO:
            return get_oloc();
            break;
        default:
            throw std::invalid_argument("Bad player in GetPosition");
    }
}

void Board::SetPosition(Player player, Point pt)
{
    switch (player) {
        case kPlayerX:
            set_xloc(pt);
            break;
        case kPlayerO:
            set_oloc(pt);
            break;
        default:
            throw std::invalid_argument("Bad player in SetPosition");
    }
}

std::unique_ptr<MovePtrVec> Board::SuccessorMoves(Player player) const
{    
    Point start = GetPosition(player);
    std::unique_ptr<MovePtrVec> moves(new MovePtrVec());

    std::shared_ptr<PointVec> pl = OpenPoints(start);
    for(auto &p : *pl) {
        MovePtr move = MovePtr(new Move(p - start, player));
        moves->push_back(move);
    }
    return moves;
}

size_t Board::NumMoves(const Player& player) const
{
    return OpenPointSearch_(GetPosition(player));
}

size_t Board::NumReachable(const Player& player) const
{
    PointSet* explored = new PointSet;
    size_t num_reachable = ExploreReachable_(GetPosition(player), explored);
    delete explored;
    return num_reachable;
}

size_t Board::ExploreReachable_(const Point& start, PointSet* explored) const
{
    explored->insert(start);
    
    // find the possible one-step moves, and sort according to distance of that
    // point from the goal (closest first)
    std::unique_ptr<PointList> lst = PointPerimeter(start);
    
    size_t size = 0;
    for (auto p : *lst) {
        if (explored->find(p) == explored->end()) {
            size += 1 + ExploreReachable_(p, explored);
        }
    }
    return size;
}


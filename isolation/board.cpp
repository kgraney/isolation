//
//  board.cpp
//  isolation
//
//  Created by Kevin Graney on 2/28/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#include <exception>

#include "board.h"
#include "point.h"
#include "move.h"

const std::initializer_list<Point> Board::kPointDirections = {
    Point(0,1), Point(1,0),
    Point(-1,0), Point(0,-1),
    Point(-1,1), Point(1,-1),
    Point(1,1), Point(-1,-1)
};

Board::Board()
: xloc_(0,0), oloc_(kSize-1,kSize-1)
{
    for (int i=0; i < kSize; i++) {
        for (int j=0; j < kSize; j++) {
            array_[i][j] = true;
        }
    }
    
    // Close the two starting points, which are directly initialized, and therefore not
    // closed by set_loc_
    ClosePoint(Point(0,0));
    ClosePoint(Point(kSize-1, kSize-1));
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
        throw std::out_of_range("Point not on the board");
}

void Board::ClosePoint(const Point& pt)
{
    if (OnBoard(pt))
        array_[pt.x()][pt.y()] = false;
    else
        throw std::out_of_range("Point not on the board");
}

std::shared_ptr<PointList> Board::OpenPoints(const Point& ref) const
{
    std::shared_ptr<PointList> l(new PointList);
    OpenPointSearch_(ref, Point(0,0), l);
    return l;
}

size_t Board::OpenPointSearch_(const Point& ref, Point direction, std::shared_ptr<PointList> lst) const
{
    if (!OnBoard(ref))
        return 0;
        
    if (direction != Point(0,0)) {
        if (PointOpen(ref)) {
            if (lst)
                lst->push_back(ref);
            return 1 + OpenPointSearch_(ref + direction, direction, lst);
        } else
            return 0;
    } else {
        size_t r = 0;
        for (auto dir : kPointDirections) {
            if (dir.x() == dir.y()) // on a diagonal
                try {
                    if (!PointOpen(ref + Point(dir.x(),0)) && !PointOpen(ref + Point(0,dir.y())))
                        continue;
                } catch(std::out_of_range) { }
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
        if (PointOpen(ref)) {
            return false;
        } else
            return true;
    } else {
        bool blocked = true;
        for (auto dir : kPointDirections) {
            if (dir.x() == dir.y()) // on a diagonal
                try {
                    if (!PointOpen(ref + Point(dir.x(),0)) && !PointOpen(ref + Point(0,dir.y())))
                        continue;
                } catch(std::out_of_range) { }
            blocked = blocked && IsPointBlocked(ref + dir, dir);
        }
        return blocked;
    }
}

std::ostream& operator<< (std::ostream& stream, const Board& board)
{
    for (int y=0; y < Board::kSize; y++) {
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
        throw std::runtime_error("Tie game?");
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
        if (OnBoard(candidate) && PointOpen(candidate) && candidate != p) {
            l->push_back(candidate);
        }
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
    //std::cout << start << explored->size() << std::endl;
    
    // find the possible one-step moves, and sort according to distance of that
    // point from the goal (closest first)
    std::unique_ptr<PointList> lst = PointPerimeter(start);
    lst->sort([&goal] (const Point& p1, const Point& p2) {
        return p1.DistanceTo(goal) < p2.DistanceTo(goal);
    });


    if (start == goal)
        return true;
    else if (lst->size() == 0)
        return false;

    bool b = false;
    for (auto p : *lst) {
        if (explored->find(p) == explored->end()) {
            b = b || SearchForPath_(p, goal, explored);
            if(b) break; // short-circuit or
        }
    }
    return b;
}

Point Board::GetPosition_(Player player) const
{
    switch (player) {
        case kPlayerX:
            return get_xloc();
            break;
        case kPlayerO:
            return get_oloc();
            break;
        default:
            throw std::invalid_argument("Bad player in GetPosition_");
    }
}

std::unique_ptr<MoveList> Board::Moves(Player player) const
{    
    Point start = GetPosition_(player);
    std::unique_ptr<MoveList> l(new MoveList());
    std::shared_ptr<PointList> pl = OpenPoints(start);
    for(auto &p : *pl) {
        l->push_back(Move(p - start, player));
    }
    return l;
}

size_t Board::NumMoves(const Player& player) const
{
    return OpenPointSearch_(GetPosition_(player));
}


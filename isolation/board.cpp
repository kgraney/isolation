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

Board::Board()
: xloc_(0,0), oloc_(kSize-1,kSize-1)
{
    for (int i=0; i < kSize; i++) {
        for (int j=0; j < kSize; j++) {
            array_[i][j] = true;
        }
    }
}

bool Board::PointOpen(const Point& pt) const
{
    if (OnBoard(pt))
        return array_[pt.x()][pt.y()];
    else
        throw std::out_of_range("Point not on the board");
}

std::ostream& operator<< (std::ostream& stream, const Board& board)
{
    for (int i=0; i < Board::kSize; i++) {
        for (int j=0; j < Board::kSize; j++) {
            Point pt(i,j);
            
            if (pt == board.get_xloc())
                stream <<'x';
            else if(pt == board.get_oloc())
                stream << 'o';
            else
                stream << (board.PointOpen(pt) ? '-' : '*');
        }
        stream << std::endl;
    }
    return stream;
}

bool Board::IsTerminalBoard()
{
    return (PointPerimeter(get_xloc())->size() == 0) ||
            (PointPerimeter(get_oloc())->size() == 0);
}

std::unique_ptr<Board::PointList> Board::PointPerimeter(const Point p) const
{
    // TODO: handle diagonal case properly
    std::unique_ptr<PointList> l(new PointList);
    for (int delta_x : {-1 , 0, 1}) {
        for (int delta_y : {-1, 0, 1}) {
            Point candidate = Point(p.x() + delta_x, p.y() + delta_y);
            if (OnBoard(candidate) && PointOpen(candidate) && candidate != p) {
                l->push_back(candidate);
            }
        }
    }
    return l;
}

bool Board::OnBoard(Point p) const
{
    return p.x() >= 0 && p.y() >= 0 && p.x() < kSize && p.y() < kSize;
}

bool Board::IsIsolatedBoard()
{
    // pick the x player to explore from, though it doesn't matter
    Point start_pos = get_xloc();
    
    PointSet* explored = new PointSet;
    bool result = SearchForPath_(start_pos, get_oloc(), explored);
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

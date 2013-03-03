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
            
            if (pt == board.xloc_)
                stream <<'x';
            else if(pt == board.oloc_)
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
    // TODO
    return true;
}

std::unique_ptr<Board::PointList> Board::PointPerimeter(const Point p) const
{
    std::unique_ptr<PointList> l(new PointList);
    for (int delta_x : {-1 , 0, 1}) {
        for (int delta_y : {-1, 0, 1}) {
            Point candidate = Point(p.x() + delta_x, p.y() + delta_y);
            if (OnBoard(candidate) && candidate != p) {
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
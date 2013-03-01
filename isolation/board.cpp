//
//  board.cpp
//  isolation
//
//  Created by Kevin Graney on 2/28/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#include "board.h"
#include "point.h"

Board::Board()
{
    for (int i=0; i < kSize; i++) {
        for (int j=0; j < kSize; j++) {
            array_[i][j] = '-';
        }
    }
    array_[0][0] = 'x';
    array_[kSize-1][kSize-1] = 'o';
}

std::ostream& operator<< (std::ostream& stream, const Board& board)
{
    for (int i=0; i < Board::kSize; i++) {
        for (int j=0; j < Board::kSize; j++) {
             stream << board.array_[i][j];
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

std::list<Point> Board::PointPerimeter(const Point p) const
{
    std::list<Point> l;
    for (int delta_x : {-1 , 0, 1}) {
        for (int delta_y : {-1, 0, 1}) {
            Point candidate = Point(p.x() + delta_x, p.y() + delta_y);
            if (OnBoard(candidate) && candidate != p) {
                l.push_back(candidate);
            }
        }
    }
    return l;
}

bool Board::OnBoard(Point p) const
{
    return p.x() >= 0 && p.y() >= 0 && p.x() < kSize && p.y() < kSize;
}
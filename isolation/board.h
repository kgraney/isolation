//
//  board.h
//  isolation
//
//  Created by Kevin Graney on 2/28/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#ifndef __isolation__board__
#define __isolation__board__

#include <ostream>
#include <list>
#include "point.h"

class Point;

class Board
{
    friend std::ostream& operator<< (std::ostream& stream, const Board& board);
    static const int kSize = 9;
    
public:
    Board();

    bool IsTerminalBoard();
    Point get_xloc() const;
    Point get_yloc() const;
    
    bool PointOpen(const Point& pt) const;
    
    // Returns a list of points surrounding the point p.  Does not include
    // points off the edge of the board.
    std::list<Point> PointPerimeter(const Point p) const;
    
private:
    
    bool OnBoard(Point p) const;
    
    // true if cell is available, false if it's blocked
    bool array_[kSize][kSize];
    
    Point xloc_; // my location
    Point oloc_; // my opponent's location

};

#endif /* defined(__isolation__board__) */

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

class Point;

class Board
{
    friend std::ostream& operator<< (std::ostream& stream, const Board& board);
    static const int kSize = 9;
    
public:
    Board();

    bool IsTerminalBoard();
    Point XLoc() const;
    Point OLoc() const;
    
    // Returns a list of points surrounding the point p.  Does not include
    // points off the edge of the board.
    std::list<Point> PointPerimeter(const Point p) const;
    
private:
    
    bool OnBoard(Point p) const;
    
    // x => the position of the 'x' player
    // o => the position of the 'o' player
    // - => an available cell
    // * => a filled cell
    char array_[kSize][kSize];
};

#endif /* defined(__isolation__board__) */

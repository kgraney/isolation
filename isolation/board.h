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
#include <set>
#include <memory>

#include "point.h"

class Point;

class Board
{
    friend std::ostream& operator<< (std::ostream&, const Board&);
    static const int kSize = 9;
    
public:
    typedef std::list<Point> PointList;
    typedef std::set<Point> PointSet;
    
    Board();

    // returns true if one of the players has won the game
    bool IsTerminalBoard();
    
    // returns true if the players are divided by filled spaces and can't interact
    // (at this point in the game we should try to not isolate ourselves)
    bool IsIsolatedBoard();
    
    inline Point get_xloc() const { return xloc_; }
    inline Point get_oloc() const { return oloc_; }
    
    void ClosePoint(const Point& pt);
    
    bool PointOpen(const Point& pt) const;
    
    // Returns a list of points surrounding the point p.  Does not include
    // points off the edge of the board or any points that are unreachable with
    // a move.
    std::unique_ptr<PointList> PointPerimeter(const Point p) const;
    
private:
    
    bool OnBoard(const Point& p) const;
    
    // true if cell is available, false if it's blocked
    bool array_[kSize][kSize];
    
    Point xloc_; // my location
    Point oloc_; // my opponent's location

    bool SearchForPath_(const Point& start, const Point& goal, PointSet*) const;
};

#endif /* defined(__isolation__board__) */

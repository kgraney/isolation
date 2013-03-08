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
#include "types.h"

class Point;

class Board
{
    friend std::ostream& operator<< (std::ostream&, const Board&);
    static const int kSize = 9;
    
public:
    
    Board();

    // returns true if one of the players has won the game
    bool IsTerminalBoard();
    
    // returns true if the players are divided by filled spaces and can't interact
    // (at this point in the game we should try to not isolate ourselves)
    bool IsIsolatedBoard();
    
    inline Point get_xloc() const { return xloc_; }
    inline Point get_oloc() const { return oloc_; }
    
    inline void set_xloc(const Point& pt) { set_loc_(pt, &xloc_); }
    inline void set_oloc(const Point& pt) { set_loc_(pt, &oloc_); }
    
    void ClosePoint(const Point& pt);
    
    bool PointOpen(const Point& pt) const;
    
    // Returns a list of points surrounding the point p.  Does not include
    // points off the edge of the board or any points that are unreachable with
    // a move.
    std::unique_ptr<PointList> PointPerimeter(const Point p) const;
    
private:
    
    bool OnBoard(const Point& p) const;
    
    void set_loc_(const Point& p, Point* loc);
    
    // true if cell is available, false if it's blocked
    bool array_[kSize][kSize];
    
    Point xloc_; // my location
    Point oloc_; // my opponent's location

    bool SearchForPath_(const Point& start, const Point& goal, PointSet*) const;
};

#endif /* defined(__isolation__board__) */

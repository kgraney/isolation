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
    static const std::initializer_list<Point> kPointDirections;
    
public:
    
    Board();

    // returns the winning player or false if no player has won yet
    Player IsTerminalBoard();
    
    // returns true if the players are divided by filled spaces and can't interact
    // (at this point in the game we should try to not isolate ourselves)
    bool IsIsolatedBoard();
    
    inline Point get_xloc() const { return xloc_; }
    inline Point get_oloc() const { return oloc_; }
    
    inline void set_xloc(const Point& pt) { set_loc_(pt, &xloc_); }
    inline void set_oloc(const Point& pt) { set_loc_(pt, &oloc_); }
    
    std::shared_ptr<PointList> OpenPoints(const Point& ref) const;
    
    void ClosePoint(const Point& pt);
    
    bool PointOpen(const Point& pt) const;
    
    // Returns a list of points surrounding the point p.  Does not include
    // points off the edge of the board or any points that are unreachable with
    // a move.
    std::unique_ptr<PointList> PointPerimeter(const Point p) const;
    
    // return a list of moves the given player can make on a board
    std::unique_ptr<MoveList> Moves(Player player) const;
    
private:
    
    bool OnBoard(const Point& p) const;
    
    Point GetPosition_(Player player) const;
    
    void set_loc_(const Point& p, Point* loc);
    
    // true if cell is available, false if it's blocked
    bool array_[kSize][kSize];
    
    Point xloc_; // my location
    Point oloc_; // my opponent's location

    bool SearchForPath_(const Point& start, const Point& goal, PointSet*) const;
    void OpenPointSearch_(const Point& ref, Point direction, std::shared_ptr<PointList> lst) const;
};

#endif /* defined(__isolation__board__) */

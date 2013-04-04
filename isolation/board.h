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
    
    // The size of the board (length of one side).
    static const int kSize = 8;

    // All valid directions we can move in from one point.
    static const std::initializer_list<Point> kPointDirections;
    
public:
    
    Board();
    Board(const Board& board);
    
    void clear();

    // returns the winning player or false if no player has won yet
    Player IsTerminalBoard();
    
    // returns true if the players are divided by filled spaces and can't interact
    // (at this point in the game we should try to not isolate ourselves)
    bool IsIsolatedBoard();
    
    inline Point get_xloc() const { return xloc_; }
    inline Point get_oloc() const { return oloc_; }
    Point GetPosition(Player player) const;
    
    inline void set_xloc(const Point& pt) { set_loc_(pt, &xloc_); }
    inline void set_oloc(const Point& pt) { set_loc_(pt, &oloc_); }
    void SetPosition(Player player, Point pt);
    
    // Return a list of points that can be moved to from the reference point
    // in a single move.
    std::shared_ptr<PointVec> OpenPoints(const Point& ref) const;
    
    // Mark the point as closed
    void ClosePoint(const Point& pt);
    
    // Return true if the point is open, false if it's closed
    bool PointOpen(const Point& pt) const;

    // Like PointOpen, but also considers the location of a player to be an
    // open point.  This is used when finding if a path to a point exists.
    bool PointLooseOpen(const Point& pt) const;
    
    // Returns a list of points surrounding the point p.  Does not include
    // points off the edge of the board or any points that are unreachable with
    // a move.
    std::unique_ptr<PointList> PointPerimeter(const Point p) const;
    
    bool IsPointBlocked(const Point& ref, Point direction = Point(0,0)) const;
    
    // return a list of moves the given player can make on a board
    std::unique_ptr<MovePtrVec> SuccessorMoves(Player player) const;

    // Return the number of moves a given player can make on this turn
    size_t NumMoves(const Player& player) const;
    
    // Return the number of spaces the given player is capable of reaching with
    // infinite moves from this position (if the rest of the board doesn't change)
    size_t NumReachable(const Player& player) const;
    
private:

    bool InvalidDiagonal(const Point& ref, const Point& direction) const;
    
    // Returns true if the point is on the board, false otherwise.
    bool OnBoard(const Point& p) const;
    
    void set_loc_(const Point& p, Point* loc);
    
    // Array storing open/closed state of board positions.  True if cell is
    // available; false if it's blocked.
    bool array_[kSize][kSize];
    
    Point xloc_; // The location of the x-player
    Point oloc_; // The location of the o-player

    // Recursive search function used by IsIsolatedBoard() that tries to find an
    // open path connecting the two players.
    bool SearchForPath_(const Point& start, const Point& goal, PointSet*) const;

    // Recursive search function used by OpenPoints() and NumMoves() that
    // searches for points reachable in a single move (DFS-like).
    size_t OpenPointSearch_(const Point& ref, Point direction = Point(0,0), std::shared_ptr<PointVec> lst = nullptr) const;

    // Recursive search function used by NumReachable() that explores all
    // points reachable in any number of moves.
    size_t ExploreReachable_(const Point& start, PointSet* explored) const;
};

#endif /* defined(__isolation__board__) */

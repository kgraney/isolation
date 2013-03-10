//
//  move.h
//  isolation
//
//  Created by Kevin Graney on 3/6/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#ifndef __isolation__move__
#define __isolation__move__

#include <iostream>

#include "point.h"
#include "types.h"

class Board;

class Move
{
    friend std::ostream& operator<< (std::ostream&, const Move&);

public:
    
    Move(int delta_x, int delta_y, Player p);
    Move(Point delta, Player pl);
    
    // Apply this move the give board, mutating it
    void ApplyToBoard(std::shared_ptr<Board> b);
    
    const Point& get_pos_delta() const { return pos_delta_; }

private:
    // Position delta, expressed as an (x,y) Point, from the current location.
    Point pos_delta_;
    
    // The player being moved
    Player player_;
    
};

#endif /* defined(__isolation__move__) */

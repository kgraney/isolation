//
//  types.h
//  isolation
//
//  Created by Kevin Graney on 3/7/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#ifndef isolation_types_h
#define isolation_types_h

#include <list>
#include <set>

class Move;
class Board;

typedef enum {
    kPlayerX,
    kPlayerO
} Player;

typedef std::list<Point> PointList;
typedef std::set<Point> PointSet;

typedef std::list<Board> BoardList;
typedef std::list<Move> MoveList;

#endif

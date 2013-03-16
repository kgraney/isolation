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

class Point;
class Move;
class Board;
class Node;

// kNoPlayer must be first so it evaluates to false
#define kPlayerXMacro \
    X(kNoPlayer, "ERROR - None") \
    X(kAllPlayers, "ERROR - All") \
    X(kPlayerX, "Player X") \
    X(kPlayerO, "Player O")

#define X(name, string) name,
typedef enum {
    kPlayerXMacro
} Player;
#undef X

std::ostream& operator<< (std::ostream& stream, const Player& player);

typedef std::shared_ptr<Node> NodePtr;
typedef std::shared_ptr<Board> BoardPtr;
typedef std::shared_ptr<Move> MovePtr;

typedef std::list<Point> PointList;
typedef std::set<Point> PointSet;

typedef std::list<Board> BoardList;
typedef std::list<Move> MoveList;
typedef std::list<Node> NodeList;

typedef std::list<NodePtr> NodePtrList;

#endif

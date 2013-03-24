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
#include <vector>
#include <deque>

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


#define kGameStateXMacro \
    X(kSparseBoard, "Sparse Board") \
    X(kMatchup, "Matchup") \
    X(kIsolated, "Isolated") \
    X(kFinished, "Finished")

#define X(name, string) name,
typedef enum {
    kGameStateXMacro
} GameState;
#undef X

std::ostream& operator<< (std::ostream& stream, const Player& player);
std::ostream& operator<< (std::ostream& stream, const GameState& player);

typedef std::shared_ptr<Node> NodePtr;
typedef std::shared_ptr<Board> BoardPtr;
typedef std::shared_ptr<Move> MovePtr;

typedef std::list<Point> PointList;
typedef std::set<Point> PointSet;

typedef std::list<Board> BoardList;
typedef std::list<Move> MoveList;
typedef std::list<Node> NodeList;

typedef std::vector<Point> PointVec;
typedef std::vector<Move> MoveVec;

typedef std::vector<MovePtr> MovePtrVec;

typedef std::list<NodePtr> NodePtrList;
typedef std::vector<NodePtr> NodePtrVec;

#endif

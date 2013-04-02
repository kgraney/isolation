//
//  node.cpp
//  isolation
//
//  Created by Kevin Graney on 3/6/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#include "node.h"

Node::Node(std::shared_ptr<Board> board, std::shared_ptr<Node> parent)
: board_(board), parent_(parent), time_expired_(false)
{
}

Node::Node(Node& node)
: board_(node.board_), parent_(node.parent_), value_(node.value_),
    time_expired_(node.time_expired_)
{
}

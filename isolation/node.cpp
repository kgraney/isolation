//
//  node.cpp
//  isolation
//
//  Created by Kevin Graney on 3/6/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#include "node.h"

Node::Node(std::shared_ptr<Board> board, std::shared_ptr<Node> parent)
: board_(board), parent_(parent)
{
}

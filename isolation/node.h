//
//  node.h
//  isolation
//
//  Created by Kevin Graney on 3/6/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#ifndef __isolation__node__
#define __isolation__node__

#include <iostream>

#include "board.h"
#include "move.h"

class Node
{
public:
    Node();
    Node(std::shared_ptr<Board> board, std::shared_ptr<Node> parent=nullptr);
    
    const std::shared_ptr<Board> get_board() { return board_; }
    
private:
    
    std::shared_ptr<Board> board_;
    std::shared_ptr<Node> parent_;
};

#endif /* defined(__isolation__node__) */

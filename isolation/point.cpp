//
//  point.cpp
//  isolation
//
//  Created by Kevin Graney on 3/1/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#include "point.h"

Point::Point()
: coord_(0,0)
{
    // initialize to (0,0)
}

Point::Point(int x, int y)
: coord_(x,y)
{
}

int Point::x() const { return coord_.first; }
int Point::y() const { return coord_.second; }

void Point::set_x(int x)
{
    coord_.first = x;
}

void Point::set_y(int y)
{
    coord_.second = y;
}

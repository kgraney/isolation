//
//  point.cpp
//  isolation
//
//  Created by Kevin Graney on 3/1/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#include <cmath>

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

void Point::set_x(int x)
{
    coord_.first = x;
}

void Point::set_y(int y)
{
    coord_.second = y;
}

double Point::DistanceTo(const Point& p) const
{
    int xdiff = x() - p.x();
    int ydiff = y() - p.y();
    return sqrt(xdiff*xdiff + ydiff*ydiff);
}

std::ostream& operator<< (std::ostream& stream, const Point& pt)
{
    stream << "(" << pt.x() << "," << pt.y() << ")";
    return stream;
}

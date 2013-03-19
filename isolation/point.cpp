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
    stream << "(" << 1+pt.x() << "," << 1+pt.y() << ")";
    return stream;
}

std::istream& operator>> (std::istream& stream, Point& pt)
{
    int x, y;
    
    std::cout << "x? ";
    stream >> x;
    pt.set_x(x-1);
    
    std::cout << "y? ";
    stream >> y;
    pt.set_y(y-1);
    
    return stream;
}

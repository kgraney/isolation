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
    // Distance from this point to another point
    int xdiff = x() - p.x();
    int ydiff = y() - p.y();
    return sqrt(xdiff*xdiff + ydiff*ydiff);
}

std::ostream& operator<< (std::ostream& stream, const Point& pt)
{
    // The (y x) format specified in the assignment.
    stream << "(" << 1+pt.y() << " " << 1+pt.x() << ")";
    return stream;
}

std::istream& operator>> (std::istream& stream, Point& pt)
{
    // Read the (y x) format specified in the assignment.  Note that
    // input is 1-based and internal storage is 0-based.
    int x, y;
    char trash;

prompt:
    std::cout << "Point in form (y x), that is (row column): ";
    stream >> trash;
    stream >> y >> x;
    stream >> trash;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore();
        goto prompt;
    }
    pt.set_x(x-1);
    pt.set_y(y-1);
    std::cin.clear();

    char c;
prompt_confirm:
    std::cout << "input point " << pt << "? [y/n]";
    std::cin >> c;
    if (c == 'y')
        return stream;
    else if(c == 'n')
        goto prompt;
    else
        goto prompt_confirm;
}

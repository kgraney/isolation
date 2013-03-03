//
//  point.h
//  isolation
//
//  Created by Kevin Graney on 3/1/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#ifndef __isolation__point__
#define __isolation__point__

#include <iostream>
#include <utility>

class Point
{
    typedef std::pair<int, int> Point_;
    
public:
    Point();
    Point(int x, int y);
    
    int x() const;
    int y() const;
    
    void set_x(int);
    void set_y(int);
    
private:
    Point_ coord_; // first = x, second = y
};


inline bool operator==(const Point& lhs, const Point& rhs)
{
    return (lhs.x() == rhs.x()) && (lhs.y() == rhs.y());
}


inline bool operator!=(const Point& lhs, const Point& rhs)
{
    return !operator==(lhs, rhs);
}


#endif /* defined(__isolation__point__) */

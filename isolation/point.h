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

    friend std::ostream& operator<< (std::ostream&, const Point&);
    friend std::istream& operator>> (std::istream&, Point&);
    
public:
    Point();
    Point(int x, int y);
    
    inline int x() const { return coord_.first; }
    inline int y() const { return coord_.second; }
    
    void set_x(int);
    void set_y(int);
    
    double DistanceTo(const Point& p) const;
    
private:
    Point_ coord_; // first = x, second = y
};


inline Point operator+(const Point& lhs, const Point& rhs)
{
    return Point(lhs.x() + rhs.x(), lhs.y() + rhs.y());
}

inline Point operator-(const Point& lhs, const Point& rhs)
{
    return Point(lhs.x() - rhs.x(), lhs.y() - rhs.y());
}

inline bool operator==(const Point& lhs, const Point& rhs)
{
    return (lhs.x() == rhs.x()) && (lhs.y() == rhs.y());
}

inline bool operator!=(const Point& lhs, const Point& rhs)
{
    return !operator==(lhs, rhs);
}

inline bool operator<(const Point& lhs, const Point& rhs)
{
    // order by x then y (need for placement in std::set
    // red-black tree)
    if (lhs.x() == rhs.x())
        return lhs.y() < rhs.y();
    else
        return lhs.x() < rhs.x();
}


#endif /* defined(__isolation__point__) */

//
// Created by kommyhap on 05.04.17.
//

#ifndef LAB5_POINT_H
#define LAB5_POINT_H
#include "color.h"

class Point
{
public:
    Point() : x(0), y(0) {}
    Point(const Point &p) : x(p.x), y(p.y) {}
    Point(const int _x, const int _y) : x(_x), y(_y) {}
    virtual ~Point() {}

    Point& operator=(const Point& p)
    {
        x = p.x;
        y = p.y;
        return *this;
    }

    inline friend bool equalCoord(const Point &p1, const Point &p2)
    { return p1.x == p2.x && p1.y == p2.y; }

    virtual inline bool operator==(const Point &p) const { return equalCoord(*this, p); }
    virtual inline bool operator!=(const Point &p) const { return !(*this == p); }

    int x, y;
};

static const Point emptyPoint = { -1, -1};
#endif //LAB5_POINT_H

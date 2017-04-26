//
// Created by kommyhap on 05.04.17.
//

#ifndef LAB5_POINT_H
#define LAB5_POINT_H
#include "color.h"

class Point
{
public:
    Point() : x(0), y(0), color(colorField) {}
    Point(const Point &p) : x(p.x), y(p.y), color(p.color) {}
    Point(int _x, int _y, Color c) : x(_x), y(_y), color(c) {}

    Point& operator=(const Point& p)
    {
        x = p.x;
        y = p.y;
        color = p.color;
        return *this;
    }

    inline friend bool equalCoord(const Point &p1, const Point &p2)
    { return p1.x == p2.x && p1.y == p2.y; }

    virtual inline bool operator==(const Point &p) const { return equalCoord(*this, p) && p.color == color; }
    virtual inline bool operator!=(const Point &p) const { return !(*this == p); }

    virtual ~Point() {}

    int x, y;
    Color color;
};

static const Point emptyPoint = { 0, 0, colorField};
#endif //LAB5_POINT_H

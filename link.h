//
// Created by kommyhap on 14.04.17.
//

#ifndef LAB6_LINK_H
#define LAB6_LINK_H

#include "point.h"

class Link : public Point
{
public:
    Link();
    Link(Point&, size_t, size_t);
    ~Link();

    Link& operator=(const Link&);

    inline bool operator==(const Link&) const;
    inline bool operator!=(const Link&) const;

    //pos[0] - pos[AREA]
    //pos[1] - pos[POLYGON]
    size_t pos[2];
};

Link& Link::operator=(const Link &l)
{
    if (*this == l) return *this;
    *dynamic_cast<Point*>(this) = *dynamic_cast<const Point*>(&l);
    this->pos[0] = l.pos[0];
    this->pos[1] = l.pos[1];
    return *this;
}

bool Link::operator==(const Link &l)
const { return equalCoord(*this, l); }

bool Link::operator!=(const Link &l)
const { return !equalCoord(*this, l);}

Link::Link(Point &_p, size_t _pos1, size_t _pos2) :
    Point(_p)
{
    pos[0] = _pos1;
    pos[1] = _pos2;
}

Link::Link() :
    Point()
{
    pos[0] = 0;
    pos[1] = 0;
}

Link::~Link() {}
#endif //LAB6_LINK_H

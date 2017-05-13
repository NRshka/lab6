//
// Created by kommyhap on 13.05.17.
//

#ifndef LAB6_PIXEL_H
#define LAB6_PIXEL_H

#include "point.h"

class Pixel : public Point
{
public:
    Pixel();
    Pixel(const int x, const int y, const Color c);
    Pixel(const Pixel&);
    ~Pixel();

    Pixel& operator=(const Pixel&);

    bool operator==(const Pixel&) const;
    bool operator!=(const Pixel&) const;

    Color c;
};

Pixel::~Pixel() {}

Pixel::Pixel() : Point(), c(colorField) {}

Pixel::Pixel(const int x, const int y, const Color c) :
    Point(x, y),
    c(c)
{}

Pixel::Pixel(const Pixel &p) : Point(p), c(p.c)
{}

Pixel& Pixel::operator=(const Pixel &p)
{
    *dynamic_cast<Point*>(this) = *dynamic_cast<const Point*>(&p);
    c = p.c;
    return *this;
}

bool Pixel::operator==(const Pixel &p) const
{
    return (*dynamic_cast<const Point*>(this) == *dynamic_cast<const Point*>(&p)
            && c == p.c);
}

bool Pixel::operator!=(const Pixel &p) const
{
    return !(*this == p);
}

static const Pixel emptyPixel = Pixel(-1, -1, colorField);
#endif //LAB6_PIXEL_H

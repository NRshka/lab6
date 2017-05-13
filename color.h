//
// Created by kommyhap on 05.04.17.
//

#ifndef LAB6_COLOR_H
#define LAB6_COLOR_H
class Color
{
public:
    unsigned char r, g, b;

    Color(unsigned char _r, unsigned char _g, unsigned char _b) : r(_r), g(_g), b(_b) {}
    Color(const Color &c) : r(c.r), g(c.g), b(c.b) {}

    bool operator==(const Color &c) const { return (r == c.r) && (g == c.g) && (b == c.b); }
    bool operator!=(const Color &c) const { return !(*this == c); }

    const Color operator=(const Color &c) {
        r = c.r;
        g = c.g;
        b = c.b;
        return *this;
    }

    ~Color() {
        r = 0xff;
        g = 0xff;
        b = 0xff;
    }
};

static const Color colorField   = { 0xff, 0xff, 0xff};
static const Color colorArea    = { 0xa8, 0xd2, 0x0c};
static const Color colorBefore  = { 0xa8, 0x54, 0xfc};
static const Color colorAfter   = { 0x2a, 0x54, 0x7e};

#endif //LAB6_COLOR_H

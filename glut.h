//
// Created by kommyhap on 05.04.17.
//
#ifndef LAB6_GLUT_H
#define LAB6_GLUT_H

#include <GL/freeglut.h>

#include "window.h"
#include "list.h"
#include "point.h"
#include "algVA.h"

class Glut
{
public:
    Glut();
    ~Glut();

    void render();

    void drawCell(Point,        bool save = true);
    void drawLine(Point, Point, bool save = true);
    void clear();

    Point          pointsScreen        [W / C][H / C];
    List<Point>    pointsPolygon       [2];
    List<Point>    pointsArea          [2];
    List<Point>    pointsCurPolygon    [2];

    //state = (точки == внешние) ? 0 : 1;
    void push(Point, bool state);

    void changeState();

private:
    void doAlgVA();
    void createListPointIntersection(List<Link>& links);
    List<Link>* checkLines(Point &x, Point &y, Point &m, Point &n);

    void getLine(Point, Point, List<Point>&);

    void clearData();

    void drawOldPoints();
    void drawGrid();
    void drawSomePolygon(List<Point>[2], Color);

    template <class T> friend void swap(T&, T&);
    inline unsigned int absolute(int);

    char state;
    enum { AREA, POLYGON, RUN };
    enum { OUTSIDE, INSIDE };
};

void Glut::doAlgVA()
{
    List<Link> pointsIntersection;
    createListPointIntersection(pointsIntersection);
    pointsIntersection;
}

void Glut::createListPointIntersection(List<Link> &pointsIntersection)
{
    List<Link> *tempList;
    Link *linkPtr = nullptr, *linkResultPtr = nullptr;
    size_t secondIndexArea, secondIndexPolygon;
    size_t incrementIndexArea = 0;
    for (unsigned char state = OUTSIDE; state <= INSIDE; ++state)
        for (size_t indexArea = 0; indexArea < pointsArea[state].size; ++indexArea)
        {
            if (incrementIndexArea)
            {
                indexArea += incrementIndexArea;
                incrementIndexArea = 0;
                if (indexArea >= pointsArea[state].size) break;
            }
            secondIndexArea = (indexArea + 1 == pointsArea[state].size) ? 0 : indexArea + 1;
            if (secondIndexArea == 0 && indexArea == 1) break;

            for (size_t indexPolygon = 0; indexPolygon < pointsPolygon[state].size; ++indexPolygon)
            {
                secondIndexPolygon = (indexPolygon + 1 == pointsPolygon[state].size) ? 0 : indexPolygon + 1;
                if (secondIndexPolygon == 0 && indexPolygon == 1) break;

                tempList = checkLines(pointsArea[state][indexArea],
                                      pointsArea[state][secondIndexArea],
                                      pointsPolygon[state][indexPolygon],
                                      pointsPolygon[state][secondIndexPolygon]);
                for (size_t indexLinks = 0; indexLinks < tempList->size; ++indexLinks)
                {
                    (*tempList)[indexLinks].pos[AREA] = secondIndexArea;
                    (*tempList)[indexLinks].pos[POLYGON] = secondIndexPolygon;

                    pointsArea[state].insert((*tempList)[indexLinks], secondIndexArea);
                    for (size_t indexCheck = secondIndexArea + 1; indexCheck < pointsArea[state].size; ++indexCheck)
                        if ((linkPtr = dynamic_cast<Link*>(&pointsArea[state][indexCheck])))
                        {
                            ++linkPtr->pos[AREA];
                            if ((linkResultPtr = pointsIntersection.find(*linkPtr))) linkResultPtr->pos[AREA] = linkPtr->pos[AREA];
                        }
                    ++incrementIndexArea;

                    pointsPolygon[state].insert((*tempList)[indexLinks], secondIndexPolygon);
                    for (size_t indexCheck = secondIndexPolygon + 1; indexCheck < pointsPolygon[state].size; ++indexCheck)
                        if ((linkPtr = dynamic_cast<Link*>(&pointsPolygon[state][indexCheck])))
                        {
                            ++linkPtr->pos[POLYGON];
                            if ((linkResultPtr = pointsIntersection.find(*linkPtr))) linkResultPtr->pos[POLYGON] = linkPtr->pos[POLYGON];
                        }
                    ++indexPolygon;

                }
                pointsIntersection += *tempList;
                delete tempList;
            }
        }
}

List<Link>* Glut::checkLines(Point &x, Point &y, Point &m, Point &n)
{
    List<Link> *result = new List<Link>();
    List<Point> line1, line2;
    getLine(x, y, line1);
    getLine(m, n, line2);
    for (size_t indexLine1 = 0; indexLine1 < line1.size; ++indexLine1)
    {
        for (size_t indexLine2 = 0; indexLine2 < line2.size; ++indexLine2)
        {
            if (equalCoord(line1[indexLine1], line2[indexLine2]))
            {
                Link *_link = new Link(line1[indexLine1], 0, 0);
                result->push(*_link);
            }
        }
    }
    return result;
}

void Glut::clear()
{
    clearData();
}

void Glut::push(Point p, bool state)
{
    switch (this->state)
    {
        case AREA:
            p.color =    colorArea;
            pointsArea   [state].push(p);
            break;
        case POLYGON:
            p.color =    colorBefore;
            pointsPolygon[state].push(p);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void Glut::changeState()
{
    if (++state == RUN)
    {
        doAlgVA();
        glutPostRedisplay();

        state = 0;
        clearData();
    }
}

void Glut::getLine(Point p1, Point p2, List<Point>& list)
{
    if (p1.x == p2.x && p1.y == p2.y) {
        if (p1.x != 0) list.push(p1);
        return;
    }
    int x0 = p1.x, y0 = p1.y;
    int x1 = p2.x, y1 = p2.y;
    bool status = 0;
    if (absolute(x1 - x0) < absolute(y1 - y0)) {
        status = 1;
        swap(x0, y0);
        swap(x1, y1);
    }

    if (x1 < x0) {
        swap(x0, x1);
        swap(y0, y1);
    }

    int deltaX = absolute(x1 - x0), deltaY = absolute(y1 - y0);
    int error = 0;
    int deltaErr = deltaY;
    int x = 0, y = y0;

    int coef = 1;
    if (y1 < y0) coef = -1;

    for (x = x0; x <= x1; ++x) {
        Point p = {x, y, p1.color};
        if (status) {
            p.y = x;
            p.x = y;
        }
        list.push(p);
        error += deltaErr;
        if (error + error >= deltaX) {
            y += coef;
            error -= deltaX;
        }
    }
}

void Glut::drawSomePolygon(List<Point> lists[2], Color color)
{
    if (lists[OUTSIDE].size) {
        glBegin(GL_POLYGON);
        glColor3ub(color.r, color.g, color.b);
        for (unsigned int i = 0; i < lists[OUTSIDE].size; ++i)
            glVertex2i(lists[OUTSIDE][i].x * C, lists[OUTSIDE][i].y * C);
        glEnd();
    }
    if (lists[INSIDE].size) {
        glBegin(GL_POLYGON);
        glColor3ub(colorField.r, colorField.g, colorField.b);
        for (unsigned int i = 0; i < lists[INSIDE].size; ++i)
            glVertex2i(lists[INSIDE][i].x * C, lists[INSIDE][i].y * C);
        glEnd();
    }
}

void Glut::render()
{
    glPushMatrix();

    drawOldPoints();

    drawSomePolygon(pointsArea, colorArea);
    drawSomePolygon(pointsPolygon, colorBefore);
    drawSomePolygon(pointsCurPolygon, colorAfter);

    if (C >= 8) drawGrid();

    glPopMatrix();
}

void Glut::drawOldPoints()
{
    for (int x = 0; x < W / C; ++x)
        for (int y = 0; y < H / C; ++y)
            drawCell(pointsScreen[x][y]);
}

void Glut::drawGrid()
{
    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    for (unsigned int i = 0; i <= W; i += C) {
        glVertex2i(i, 0);
        glVertex2i(i, H);
        glVertex2i(0, i);
        glVertex2i(W, i);
    }
    glEnd();
}

unsigned int Glut::absolute(int a)
{
    return static_cast<unsigned int>((a < 0) ? -a : a);
}

void Glut::clearData()
{
    for (int x = 0; x < W / C; ++x)
        for (int y = 0; y < H / C; ++y)
            pointsScreen[x][y] = {x, y, colorField };

    pointsPolygon[ INSIDE].clear();
    pointsPolygon[OUTSIDE].clear();
    pointsArea   [ INSIDE].clear();
    pointsArea   [OUTSIDE].clear();

    state = AREA;
}

template <class T>
void swap(T& t1, T& t2)
{
    T temp = T(t1);
    t1 = t2;
    t2 = temp;
}

void Glut::drawCell(Point p, bool save)
{
    if (p.x >= 0 && p.x < (W / C) && p.y >= 0 && p.y < (H / C)) {
        if (save) pointsScreen[p.x][p.y].color = p.color;
        glColor3ub(p.color.r, p.color.g, p.color.b);
        glBegin(GL_QUADS);
        glVertex2i(p.x * C + C, p.y * C);
        glVertex2i(p.x * C, p.y * C);
        glVertex2i(p.x * C, p.y * C + C);
        glVertex2i(p.x * C + C, p.y * C + C);
        glEnd();
    }
}

void Glut::drawLine(Point p1, Point p2, bool save)
{
    List<Point> list;
    getLine(p1, p2, list);
    for (unsigned int i = 0; i < list.size; ++i)
    {
        list[i].color = p1.color;
        drawCell(list[i], save);
    }
}

Glut::Glut() {
    clearData();
}

Glut::~Glut() {
    clearData();
}
#endif //LAB6_GLUT_H

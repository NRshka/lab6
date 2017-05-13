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

    void drawCell(const Point&,               bool save = true);
    void drawLine(const Point&, const Point&, bool save = true);
    void clear();

    Point          pointsScreen        [W / C][H / C];
    List<Point>    pointsPolygon;
    List<Point>    pointsArea   ;
    List<List<Point>>    pointsCurPolygon;

    //state = (точки == внешние) ? 0 : 1;
    void push(Point&);

    void changeState();

private:
    void doAlgVA();
    void createListPointIntersection(List<Link>& links);
    void findCurrentPoints(List<List<Point>>& pointsResult, const List<Link>& pointsIntersection);
    List<Link>* checkLines(const Point &x, const Point &y, const Point &m, const Point &n);

    void getLine(const Point&, const Point&, List<Point>&);

    void clearData();

    void drawOldPoints();
    void drawGrid();
    void drawSomePolygon(const List<Point>, const Color&);

    template <class T> friend void swap(T&, T&);
    inline unsigned int absolute(const int);

    char state;
    enum { AREA, POLYGON, RUN };
};

void Glut::doAlgVA()
{
    List<Link> pointsIntersection;
    createListPointIntersection(pointsIntersection);

    findCurrentPoints(pointsCurPolygon, pointsIntersection);
}

void Glut::findCurrentPoints(List<List<Point>>& pointsResult, const List<Link>& pointsIntersection)
{
    List<Point> somePartFinalPolygon;
    Point* pointAdded;
    Point startPoint;
    size_t pos;
    for (size_t index = 0; index < pointsIntersection.size; ++index)
    {
        Link linkCmp = (pointsIntersection.size == 1) ? pointsIntersection[0] :
                       (index != pointsIntersection.size - 1) ? pointsIntersection[index + 1] :
                       pointsIntersection[0];
        pointAdded = &pointsIntersection[index];
        startPoint = *pointAdded;
        try {
            do {
                if (dynamic_cast<Link *>(pointAdded)->pos[POLYGON])
                    pos = dynamic_cast<Link *>(pointAdded)->pos[POLYGON];
                else throw 42;

                do {
                    somePartFinalPolygon.push(*pointAdded);
                    if (++pos == pointsPolygon.size) pos = 0;
                    pointAdded = &pointsPolygon[pos];
                } while (!dynamic_cast<Link *>(pointAdded));

                if (dynamic_cast<Link *>(pointAdded)->pos[AREA])
                    pos = dynamic_cast<Link *>(pointAdded)->pos[AREA];
                else throw 42;

                do {
                    somePartFinalPolygon.push(*pointAdded);
                    if (++pos == pointsArea.size) pos = 0;
                    pointAdded = &pointsArea[pos];
                } while (!dynamic_cast<Link *>(pointAdded));
            } while (!equalCoord(startPoint, *pointAdded));
        }
        catch (...) { break; }

        pointsResult.push(somePartFinalPolygon);
    }
}

void Glut::createListPointIntersection(List<Link> &pointsIntersection)
{
    List<Link> *tempList;
    Link *linkPtr = nullptr, *linkResultPtr = nullptr;
    size_t secondIndexArea, secondIndexPolygon;
    size_t incrementIndexArea = 0;
    for (size_t indexArea = 0; indexArea < pointsArea.size; ++indexArea)
    {
        if (incrementIndexArea)
        {
            indexArea += incrementIndexArea;
            incrementIndexArea = 0;
            if (indexArea >= pointsArea.size) break;
        }
        secondIndexArea = (indexArea + 1 == pointsArea.size) ? 0 : indexArea + 1;
        if (secondIndexArea == 0 && indexArea == 1) break;

        for (size_t indexPolygon = 0; indexPolygon < pointsPolygon.size; ++indexPolygon)
        {
            secondIndexPolygon = (indexPolygon + 1 == pointsPolygon.size) ? 0 : indexPolygon + 1;
            if (secondIndexPolygon == 0 && indexPolygon == 1) break;

            tempList = checkLines(pointsArea[indexArea],
                                  pointsArea[secondIndexArea],
                                  pointsPolygon[indexPolygon],
                                  pointsPolygon[secondIndexPolygon]);
            for (size_t indexLinks = 0; indexLinks < tempList->size; ++indexLinks)
            {
                (*tempList)[indexLinks].pos[AREA] = secondIndexArea;
                (*tempList)[indexLinks].pos[POLYGON] = secondIndexPolygon;

                pointsArea.insert((*tempList)[indexLinks], secondIndexArea);
                for (size_t indexCheck = secondIndexArea + 1; indexCheck < pointsArea.size; ++indexCheck)
                    if ((linkPtr = dynamic_cast<Link*>(&pointsArea[indexCheck])))
                    {
                        ++linkPtr->pos[AREA];
                        if ((linkResultPtr = pointsIntersection.find(*linkPtr))) linkResultPtr->pos[AREA] = linkPtr->pos[AREA];
                    }
                ++incrementIndexArea;

                pointsPolygon.insert((*tempList)[indexLinks], secondIndexPolygon);
                for (size_t indexCheck = secondIndexPolygon + 1; indexCheck < pointsPolygon.size; ++indexCheck)
                    if ((linkPtr = dynamic_cast<Link*>(&pointsPolygon[indexCheck])))
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

List<Link>* Glut::checkLines(const Point &x, const Point &y, const Point &m, const Point &n)
{
    List<Link> *result = new List<Link>();
    if (equalCoord(x, m) && equalCoord(y, n)) return result;
    List<Point> line1, line2;
    getLine(x, y, line1);
    getLine(m, n, line2);
    for (size_t indexLine1 = 0; indexLine1 < line1.size; ++indexLine1)
    {
        for (size_t indexLine2 = 0; indexLine2 < line2.size; ++indexLine2)
        {
            if (equalCoord(line1[indexLine1], line2[indexLine2]))
                result->push(Link(line1[indexLine1], 0, 0));
        }
    }
    return result;
}

void Glut::clear()
{
    clearData();
}

void Glut::push(Point &p)
{
    switch (this->state)
    {
        case AREA:
            pointsArea   .push(p);
            break;
        case POLYGON:
            pointsPolygon.push(p);
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
        state = 0;
    }
}

void Glut::getLine(const Point &p1, const Point &p2, List<Point>& list)
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
        Point p = {x, y};
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

void Glut::drawSomePolygon(const List<Point> list, const Color &color)
{
    if (list.size) {
        glBegin(GL_POLYGON);
        glColor3ub(color.r, color.g, color.b);
        for (unsigned int i = 0; i < list.size; ++i)
            glVertex2i(list[i].x * C, list[i].y * C);
        glEnd();
    }
}

void Glut::render()
{
    glPushMatrix();

    drawOldPoints();

    drawSomePolygon(pointsArea, colorArea);
    drawSomePolygon(pointsPolygon, colorBefore);
    for (size_t i = 0; i < pointsCurPolygon.size; ++i)
        drawSomePolygon(pointsCurPolygon[i], colorAfter);

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

unsigned int Glut::absolute(const int a)
{
    return static_cast<unsigned int>((a < 0) ? -a : a);
}

void Glut::clearData()
{
    for (int x = 0; x < W / C; ++x)
        for (int y = 0; y < H / C; ++y)
            pointsScreen[x][y] = {x, y, colorField };

    pointsPolygon.clear();
    pointsArea   .clear();

    state = AREA;
}

template <class T>
void swap(T& t1, T& t2)
{
    T temp = T(t1);
    t1 = t2;
    t2 = temp;
}

void Glut::drawCell(const Point &p, bool save)
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

void Glut::drawLine(const Point &p1, const Point &p2, bool save)
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

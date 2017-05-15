#include <stdlib.h> 
#include <windows.h>
#include <list>
#include <iostream> 
#include <glut.h>
#include <cmath> 

using namespace std;

struct point {
	double x;
	double y;
	int index = -1;
};
struct lines {
	point p1;
	point p2;
};

int indGo = 0;

//int map[100][100];
bool wac = 0;
const int view_WinWH = 40;
const int period = 40;

const int WinWH = 500;

int countP = 0;
int countP2 = 0;
int xe = 0;


point pointP_l[199];
point pointP_l2[199];
lines forTect;

int gi = 0;
int gi2 = 0;

struct color
{
	double r, g, b, a;
	void setThis() const
	{
		const color &color = *this;
		glColor4d(color.r, color.g, color.b, color.a);
	}
};

const color black = { 0, 0, 0, 1 };
const color blue = { 0, 0.1, 0.8, 1 };
const color green = { 0.5, 1.0, 0.0, 1 };
const color red = { 1, 0, 0, 1 };
const color orange = { 1, 0.5, 0.0, 1 };
const color grey = { 0.2, 0.2, 0.2, 0.5 };
const color white = { 1.0, 1.0, 1.0, 1.0 };

void DrawPixel(int x, int y) //"ставим" точку
{
	//glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2d(x - 0.5, y - 0.5);
	glVertex2d(x - 0.5, y + 0.5);
	glVertex2d(x + 0.5, y + 0.5);
	glVertex2d(x + 0.5, y - 0.5);
	glEnd();
}

inline int area(point a, point b, point c) {
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

inline bool intersect_1(int a, int b, int c, int d) {
	if (a > b)  swap(a, b);
	if (c > d)  swap(c, d);
	return max(a, c) <= min(b, d);
}

bool intersect(point a, point b, point c, point d) {
	return intersect_1(a.x, b.x, c.x, d.x)
		&& intersect_1(a.y, b.y, c.y, d.y)
		&& area(a, b, c) * area(a, b, d) <= 0
		&& area(c, d, a) * area(c, d, b) <= 0;
}

point pointOfInter(lines one, lines two) {
	if ((two.p1.x == two.p2.x) && (one.p1.x != one.p2.x)) {
		double a1 = (one.p2.y - one.p1.y) / (one.p2.x - one.p1.x);
		double b1 = one.p1.y - one.p1.x*a1;
		double x = two.p1.x;
		double y = a1*x + b1;
		point an;
		an.x = roundf(x);
		an.y = roundf(y);
		return an;
	}
	if ((one.p1.x == one.p2.x) && (two.p1.x != two.p2.x)) {
		double a1 = (two.p2.y - two.p1.y) / (two.p2.x - two.p1.x);
		double b1 = two.p1.y - two.p1.x*a1;
		double x = one.p1.x;
		double y = a1*x + b1;
		point an;
		an.x = roundf(x);
		an.y = roundf(y);
		return an;
	}
	if ((one.p1.x == one.p2.x) && (two.p1.x == two.p2.x)) {
		point an;
		an.index = -2;//не пересекаетс€ либо только касаетс€
		return an;
	}
	double a1 = (one.p2.y - one.p1.y) / (one.p2.x - one.p1.x);
	double a2 = (two.p2.y - two.p1.y) / (two.p2.x - two.p1.x);
	double b1 = one.p1.y - one.p1.x*a1;
	double b2 = two.p1.y - two.p1.x*a2;
	double x = (b2 - b1) / (a1 - a2);
	double y = a1*x + b1;
	point an;
	an.x = roundf(x);
	an.y = roundf(y);
	return an;
}

void DrawNet() //прорисовка сетки координат
{
	glLineWidth(1);
	black.setThis();

	for (double i = -view_WinWH; i <= view_WinWH; i += 1.0)
	{

		glBegin(GL_LINES);
		glVertex2f(i + 1.f, -view_WinWH + .0f);
		glVertex2f(i + 1., view_WinWH + .0f);
		glVertex2f(-view_WinWH + .0f, i + 1.f);
		glVertex2f(view_WinWH + .0f, i + 1.f);
		glEnd();

	}
	glLineWidth(2);
	blue.setThis();
	glBegin(GL_LINES);
	glVertex2i(-view_WinWH, 0);
	glVertex2i(view_WinWH, 0);
	glVertex2i(0, -view_WinWH);
	glVertex2i(0, view_WinWH);
	glEnd();
}

void DrawLineBrez(int x0, int y0, int x1, int y1, int flag) //рисуем многоугольник по точкам
{
	bool steep = false;
	if (std::abs(x0 - x1)<std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0>x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	int i = 0;
	for (int x = x0; x <= x1; x++)
	{
		if (steep)
		{
			DrawPixel(y, x);
		}
		else
		{
			DrawPixel(x, y);
		}

		error2 += derror2;

		if (error2>dx)
		{
			y += (y1>y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}


bool DrawPolygon()
{
	int numberOf = 0;
	int i, j;
	bool rez = 0;
	for (i = 0; i < countP - 1; i++) {
		for (j = 0; j < countP2 - 1; j++) {
			if (intersect(pointP_l[i], pointP_l[i + 1], pointP_l2[j], pointP_l2[j + 1])) {
				point xe = pointOfInter({ pointP_l[i],pointP_l[i + 1] }, { pointP_l2[j],pointP_l2[j + 1] });
				if (xe.index == -2) 
					continue;
				if (xe.x != pointP_l[i].x || xe.y != pointP_l[i].y) {//что б исключить повторение нахождени€ одной и той же точки пересечени€
					rez = 1;
					for (int k = countP - 1; k > i; k--) {//сдвиг
						pointP_l[k + 1] = pointP_l[k];
					}
					for (int k = countP2 - 1; k > j; k--) {
						pointP_l2[k + 1] = pointP_l2[k];
					}
					if (indGo == 0)
						indGo = i + 1;
					countP++;
					xe.index = numberOf;
					pointP_l[i + 1] = xe;
					xe.index = numberOf++;
					pointP_l2[j + 1] = xe;
					countP2++;
					j++;
					i++;
				}
			}
		}
		if (intersect(pointP_l[i], pointP_l[i + 1], pointP_l2[j], pointP_l2[0])) {//замкнуть j отсекающею фигуру
			point xe = pointOfInter({ pointP_l[i],pointP_l[i + 1] }, { pointP_l2[j],pointP_l2[0] });
			if (xe.index == -2) continue;
			if (xe.x != pointP_l[i].x || xe.y != pointP_l[i].y) {//что б исключить повторение нахождени€ одной и той же точки пересечени€
				rez = 1;
				for (int k = countP - 1; k > i; k--) {//сдвиг
					pointP_l[k + 1] = pointP_l[k];
				}
				for (int k = countP2 - 1; k > j; k--) {
					pointP_l2[k + 1] = pointP_l2[k];
				}
				if (indGo == 0)
					indGo = i + 1;
				countP++;
				xe.index = numberOf;
				pointP_l[i + 1] = xe;
				xe.index = numberOf++;
				pointP_l2[j + 1] = xe;
				countP2++;
				j++;
				//i++;
			}
		}
	}
	for (j = 0; j < countP; j++) {
		if (intersect(pointP_l[i], pointP_l[0], pointP_l2[j], pointP_l2[j + 1])) {
			point xe = pointOfInter({ pointP_l[i],pointP_l[0] }, { pointP_l2[j],pointP_l2[j + 1] });
			if (xe.index == -2) continue;
			if (xe.x != pointP_l[i].x || xe.y != pointP_l[i].y) {//что б исключить повторение нахождени€ одной и той же точки пересечени€
				rez = 1;
				for (int k = countP - 1; k > i; k--) {//сдвиг
					pointP_l[k + 1] = pointP_l[k];
				}
				for (int k = countP2 - 1; k > j; k--) {
					pointP_l2[k + 1] = pointP_l2[k];
				}
				if (indGo == 0)
					indGo = i + 1;
				countP++;
				xe.index = numberOf;
				pointP_l[i + 1] = xe;
				xe.index = numberOf++;
				pointP_l2[j + 1] = xe;
				countP2++;
				i++;
			}
		}
	}
	pointP_l[countP] = pointP_l[0];
	pointP_l2[countP2] = pointP_l2[0];
	return rez;
}

int circleCum(int i, int di, int maxI) {//LOL
	if (i + di > maxI)
		return i + di - maxI;
	if (i + di < 0)
		return maxI + i + di;
	return i + di;
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	DrawNet();

	if (gi + gi2 + 1 > countP + countP2) {//это тупо отрисовка, можешь пропустить
		bool in21 = 0;
		bool in12 = 0;
		if (!DrawPolygon()) {
			forTect.p1 = pointP_l2[0];
			forTect.p2.x = pointP_l2[0].x + 100;
			forTect.p2.y = pointP_l2[0].y;
			for (int i = 0; i < countP; i++) {
				if (intersect(forTect.p1, forTect.p2, pointP_l[i], pointP_l[i + 1])) {//второй лежит в первом, если луч, направленнй вправо пересекает пр€моугольник нечетное кол-во раз
					in21 = !in21;
				}
			}
			if (in21) {
				for (int i = 0; i < countP2; i++) {
					green.setThis();
					DrawLineBrez(pointP_l2[i].x, pointP_l2[i].y, pointP_l2[i + 1].x, pointP_l2[i + 1].y, 1);
					glFlush();
				}
				for (int i = 0; i < countP; i++) {
					blue.setThis();
					DrawLineBrez(pointP_l[i].x, pointP_l[i].y, pointP_l[i + 1].x, pointP_l[i + 1].y, 1);
					glFlush();
				}
			}
			else
			{
				forTect.p1 = pointP_l[0];
				forTect.p2.x = pointP_l[0].x + 100;
				forTect.p2.y = pointP_l[0].y;
				for (int i = 0; i < countP; i++) {
					if (intersect(forTect.p1, forTect.p2, pointP_l2[i], pointP_l2[i + 1])) {//второй лежит в первом, если луч, направленнй вправо пересекает пр€моугольник нечетное кол-во раз
						in12 = !in12;
					}
				}
				if (in12) {
					for (int i = 0; i < countP2; i++) {
						blue.setThis();
						DrawLineBrez(pointP_l2[i].x, pointP_l2[i].y, pointP_l2[i + 1].x, pointP_l2[i + 1].y, 1);
						glFlush();
					}
					for (int i = 0; i < countP; i++) {
						green.setThis();
						DrawLineBrez(pointP_l[i].x, pointP_l[i].y, pointP_l[i + 1].x, pointP_l[i + 1].y, 1);
						glFlush();
					}
				}
			}
			if (!in21 && !in12) {
				for (int i = 0; i < countP2; i++) {
					blue.setThis();
					DrawLineBrez(pointP_l2[i].x, pointP_l2[i].y, pointP_l2[i + 1].x, pointP_l2[i + 1].y, 1);
					glFlush();
				}
				for (int i = 0; i < countP; i++) {
					DrawLineBrez(pointP_l[i].x, pointP_l[i].y, pointP_l[i + 1].x, pointP_l[i + 1].y, 1);
					glFlush();
				}
			}
		}
		else
		{
			bool z = 0;
			forTect.p1 = pointP_l2[0];
			forTect.p2.x = pointP_l2[0].x + 100;
			forTect.p2.y = pointP_l2[0].y;
			for (int i = 0; i < countP; i++) {
				if (intersect(forTect.p1, forTect.p2, pointP_l[i], pointP_l[i + 1])) {//второй лежит в первом, если луч, направленнй вправо пересекает пр€моугольник нечетное кол-во раз
					in21 = !in21;
				}
			}
			for (int i = 0; i < countP; i++) {
				if (pointP_l[i].index != -1)
					z = !z;
				if (!z)
					blue.setThis();
				else
					green.setThis();
				DrawLineBrez(pointP_l[i].x, pointP_l[i].y, pointP_l[i + 1].x, pointP_l[i + 1].y, 1);
				glFlush();
			}
			//DrawLineBrez(pointP_l[countP - 1].x, pointP_l[countP - 1].y, pointP_l[0].x, pointP_l[0].y, 1);
			glFlush();
			z = 0;
			if (in21)
				z = 1;
			for (int i = 0; i < countP2; i++) {
				if (pointP_l2[i].index != -1)
					z = !z;
				if (!z)
					blue.setThis();
				else
					green.setThis();
				DrawLineBrez(pointP_l2[i].x, pointP_l2[i].y, pointP_l2[i + 1].x, pointP_l2[i + 1].y, 1);
				glFlush();
			}
			//DrawLineBrez(pointP_l2[countP - 1].x, pointP_l2[countP - 1].y, pointP_l2[0].x, pointP_l2[0].y, 1);
			glFlush();
			///////////////////////////////////////////////////////////////
			//это отсечение
			/*bool z = 0;
			int i = indGo;
			do{
				if (pointP_l[i].index != -1 && !z) {
					z = 1;
					int numberOf = pointP_l[i].index;
					int j = 0;
					while (pointP_l2[j++].index != numberOf);
					i = j - 1;
					goto draw1;
				}
				if (pointP_l2[i].index != -1 && z) {
					z = 0;
					int numberOf = pointP_l[i].index;
					int j = 0;
					while (pointP_l[j++].index != numberOf);
					i = j - 1;
					goto draw2;
				}
				if (!z) {
					draw2:
					blue.setThis();
					DrawLineBrez(pointP_l[i].x, pointP_l[i].y, pointP_l[circleCum(i, 1, countP)].x, pointP_l[circleCum(i, 1, countP)].y, 1);
					glFlush();
					i = circleCum(i, 1, countP);
				}
				if (z) {
					draw1:
					green.setThis();
					DrawLineBrez(pointP_l2[i].x, pointP_l2[i].y, pointP_l2[circleCum(i, -1, countP2)].x, pointP_l2[circleCum(i, -1, countP2)].y, 1);
					i = circleCum(i, -1, countP2);
					glFlush();
				}

			} while (i != indGo);*/

			/*for (int i = 0; i < countP - 1; i++) {
				if (z) {
					green.setThis();
					DrawLineBrez(pointP_l[i].x, pointP_l[i].y, pointP_l[i + 1].x, pointP_l[i + 1].y, i << 1);
					if (pointP_l[i + 1].index != -1) {
						z = 0;
						i = pointP_l[i + 1].index - 1;
					}
				}
				else
				{
					blue.setThis();
					DrawLineBrez(pointP_l2[i].x, pointP_l2[i].y, pointP_l2[i + 1].x, pointP_l2[i + 1].y, i << 1);
					if (pointP_l2[i + 1].index != -1) {
						z = 1;
						i = pointP_l2[i + 1].index - 1;
					}
				}
				glFlush();
			}
			DrawLineBrez(pointP_l[countP - 1].x, pointP_l[countP - 1].y, pointP_l[0].x, pointP_l[0].y, 1 << 1);
			glFlush();*/
		}
	}

	glFlush();
}

void timf(int value)
{
	glutPostRedisplay();
	glutTimerFunc(period, timf, 0);
}

void Initialize()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-view_WinWH, view_WinWH, -view_WinWH, view_WinWH, -1.0, 1.0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WinWH, WinWH);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Lab6");
	Initialize();

	int x, y;
	cin >> countP;
	while (gi<countP) {
		printf("ђведите координат точки многоугольника(x,y): ");
		cin >> x >> y;
		point an;
		an.x = x;
		an.y = y;
		//map[x + 50][y + 50] = 1;
		pointP_l[gi++] = an;;
	}
	cin >> countP2;
	while (gi2<countP2) {
		printf("ђведите координат точки многоугольника(x,y): ");
		cin >> x >> y;
		point an;
		an.x = x;
		an.y = y;
		//map[x + 50][y + 50] = 1;
		pointP_l2[gi2++] = an;;
	}

	//cin >> line.p1.x >> line.p1.y >> line.p2.x >> line.p2.y;
	glutDisplayFunc(Display);
	//glutTimerFunc(period, timf, 0);
	glutMainLoop();
}

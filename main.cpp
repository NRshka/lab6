#include <bits/unique_ptr.h>
#include "glut.h"

void mouse(int, int, int, int);
void keyboard(unsigned char, int, int);
void render();

Glut *glut = nullptr;

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glut->render();
    glutSwapBuffers();
}

/*
 * На левую кнопку идет добавление внешних точек,
 * на правую - внутренних.
 */
void mouse(int button, int state, int x, int y) {
    if (state == GLUT_UP)
    {
        Point p(x / C, y / C, colorField);
        glut->push(p, button == GLUT_RIGHT_BUTTON);
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key)
    {
        case 0x1B:
            glutLeaveMainLoop();
            break;
        case 'c': case 'C': //eng
            glut->clear();
            glutPostRedisplay();
            break;
        case ' ':
            glut->changeState();
            break;
        default:
            break;
    }
}

void test()
{
//    List<List<int>> *l = new List<List<int>>();
//    List<int> *tempList = new List<int>();
//    int tempInt = 2;
//    tempList->push(tempInt);
//    tempList->push(tempInt + 1);
//    tempList->push(tempInt + 2);
//    l->push(*tempList);
//    delete tempList;
//    delete l;

    glut->push(*(std::unique_ptr<Point>(new Point(3, 3, colorArea))), 0);
    glut->push(*(std::unique_ptr<Point>(new Point(8, 3, colorArea))), 0);
    glut->push(*(std::unique_ptr<Point>(new Point(8, 8, colorArea))), 0);
    glut->push(*(std::unique_ptr<Point>(new Point(3, 8, colorArea))), 0);

    glut->changeState();

    glut->push(*(std::unique_ptr<Point>(new Point(4, 2, colorBefore))), 0);
    glut->push(*(std::unique_ptr<Point>(new Point(10, 2, colorBefore))), 0);
    glut->push(*(std::unique_ptr<Point>(new Point(10, 5, colorBefore))), 0);
    glut->push(*(std::unique_ptr<Point>(new Point(4, 5, colorBefore))), 0);

    glut->changeState();
}

int main(int argc, char **argv) {
    glut = new Glut();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(W, H);
    glutCreateWindow("Michael Bontzio (RGR)");
    glClearColor(colorField.r, colorField.g, colorField.b, 0xFF);
    glMatrixMode(GL_PROJECTION);
    glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );
    glLoadIdentity();
    glOrtho(0, W, H, 0, 1, 0);

    glutDisplayFunc(render);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    test();

    glutMainLoop();

    delete glut;
    return 0;
}
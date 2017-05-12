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
    glut->push(*std::unique_ptr<Point>(new Point(3, 2, colorArea)), 0);
    glut->push(*std::unique_ptr<Point>(new Point(7, 2, colorArea)), 0);
    glut->push(*std::unique_ptr<Point>(new Point(9, 5, colorArea)), 0);
    glut->push(*std::unique_ptr<Point>(new Point(9, 9, colorArea)), 0);
    glut->push(*std::unique_ptr<Point>(new Point(5, 10, colorArea)), 0);
    glut->push(*std::unique_ptr<Point>(new Point(1, 6, colorArea)), 0);

    glut->changeState();

    glut->push(*std::unique_ptr<Point>(new Point(6, 0, colorBefore)), 0);
    glut->push(*std::unique_ptr<Point>(new Point(9, 2, colorBefore)), 0);
    glut->push(*std::unique_ptr<Point>(new Point(9, 5, colorBefore)), 0);
    glut->push(*std::unique_ptr<Point>(new Point(6, 4, colorBefore)), 0);
    glut->push(*std::unique_ptr<Point>(new Point(4, 4, colorBefore)), 0);
    glut->push(*std::unique_ptr<Point>(new Point(5, 2, colorBefore)), 0);

    /*
    glut->push(Point(4, 1, colorBefore), 0);
    glut->push(Point(6, 1, colorBefore), 0);
    glut->push(Point(6, 4, colorBefore), 0);
    glut->push(Point(4, 4, colorBefore), 0);
    */
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
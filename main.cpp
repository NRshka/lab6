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
    glut->push(Point(0, 0, colorArea), 0);
    glut->push(Point(6 * C, 0, colorArea), 0);
    glut->push(Point(6 * C, 6 * C, colorArea), 0);
    glut->push(Point(0, 6 * C, colorArea), 0);

    glut->changeState();

    glut->push(Point(1 * C, 8 * C, colorBefore), 0);
    glut->push(Point(8 * C, 1 * C, colorBefore), 0);
    glut->push(Point(8 * C, 8 * C, colorBefore), 0);

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
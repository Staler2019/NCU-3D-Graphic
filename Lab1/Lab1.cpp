// first
#include <GL/glut.h>  // need to be first include
// second
#include <GL/gl.h>  // need to be the second

#include <algorithm>
#include <cmath>
#include <ctime>  // strand
#include <iostream>
#include <vector>

#include "BasicGraph/Circle.h"
#include "BasicGraph/Line.h"
#include "BasicGraph/Point.h"
#include "BasicGraph/Poly.h"
#include "BasicGraph/RGBCode.h"
#include "BasicGraph/Util.h"

int window;
GLubyte window_pixels[3 * WINDOW_WIDTH * WINDOW_HEIGHT];
GLubyte tmp_window_pixels[3 * WINDOW_WIDTH * WINDOW_HEIGHT];
bool line_mode = 0;
bool circle_mode = 0;
// bool poly_mode = 0;
std::vector<Poly> poly_list;
Point last_point;
RGBCode global_rgb;  // global rgb
unsigned char key_mode = 0;
bool dot_drag_mode = 0;

// my func
void clearView();
// inline int myFloor(float);

// glut inherit func
void display();
void keyDown(unsigned char key, int x, int y);
void mouseDrag(int x, int y);
void mouse(int bin, int state, int x, int y);

int main(int argc, char* argv[]) {
    srand(time(NULL));  // random

    glutInit(&argc, argv);
    // system("pause");

    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);  // ?
    window = glutCreateWindow("My Lab2 Window!");
    glutSwapBuffers();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseDrag);
    glutKeyboardFunc(keyDown);
    // glutKeyboardUpFunc(keyUp);
    // glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF); // disable auto repeat:
    // https://stackoverflow.com/questions/39561997/glut-holding-a-key-down
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);  // ?
    glutMainLoop();

    return 0;
}

void display() {
    // glClearColor(0.0, 0.0, 0.0, 0.0);
    // glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    return;
}

void mouse(int bin, int state, int x, int y) {
    Point now_point(x, y);

    if (state == GLUT_DOWN) {
        glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB,
                     GL_UNSIGNED_BYTE, window_pixels);

        if (bin == GLUT_LEFT_BUTTON) {
            // change tmp_rgb when first trigger a pattern
            switch (key_mode) {
                case 'd':
                case 'D': {
                    dot_drag_mode = 1;
                    global_rgb = RGBCode();
                    Point(x, y, global_rgb, DEFAULT_SIZE)
                        .draw();  // Line needs 2 points

                    break;
                }
                case 'l':
                case 'L': {
                    line_mode ^= 1;
                    if (!line_mode)
                        Line(last_point, now_point, RGBCode(), DEFAULT_SIZE)
                            .draw();

                    break;
                }
                case 'o':
                case 'O': {
                    circle_mode ^= 1;
                    if (!circle_mode)
                        Circle(last_point, now_point, RGBCode(), DEFAULT_SIZE)
                            .draw();

                    break;
                }
                case 'p':
                case 'P': {
                    if (poly_list.size() == 0 || poly_list.back().isEnded())
                        poly_list.emplace_back(now_point, RGBCode(),
                                               DEFAULT_SIZE);
                    else
                        poly_list.back().addPoint(now_point);

                    break;
                }
                case 'c':
                case 'C': {
                    std::cout << "Clear the view\n";

                    break;
                }
                default: {
                    std::cerr << "key_mode: \"" << key_mode
                              << "\" is not a recognizable command\n";

                    break;
                }
            }
            last_point.setXY(x, y);
        } else if (bin == GLUT_RIGHT_BUTTON)
            if (key_mode == 'p' || key_mode == 'P') poly_list.back().end();
    } else if (state == GLUT_UP)
        if (bin == GLUT_LEFT_BUTTON)
            if (key_mode == 'd' || key_mode == 'D') dot_drag_mode = 0;

    return;
}

void keyDown(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
        case 'Q': {
            glutDestroyWindow(window);  // exit(0);

            break;  // perhaps do not need this?
        }
        case 'r':
        case 'R': {  // reverse: need to store the last scene
            // Quest. and also "rr" result to now scene? -> "Yes"

            glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB,
                         GL_UNSIGNED_BYTE, tmp_window_pixels);
            glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE,
                         window_pixels);
            glFinish();
            std::copy(tmp_window_pixels,
                      tmp_window_pixels + 3 * WINDOW_WIDTH * WINDOW_HEIGHT,
                      window_pixels);

            break;
        }
        case 'c':
        case 'C': {
            clearView();

            break;
        }
        default: {
            if (key_mode == key) break;  // keyboard the same
            key_mode = key;

            // key_mode init
            switch (key) {
                case 'l':
                case 'L': {
                    line_mode = 0;

                    break;
                }
                case 'o':
                case 'O': {
                    circle_mode = 0;

                    break;
                }
                case 'p':
                case 'P': {
                    if (poly_list.size() > 0 && !poly_list.back().isEnded())
                        poly_list.pop_back();

                    break;
                }
            }

            break;
        }
    }

    return;
}

void mouseDrag(int x, int y) {  // seen this as a loop
    if (dot_drag_mode) {
        Line(last_point, Point(x, y), global_rgb, DEFAULT_SIZE)
            .draw();          // TODO. maybe we should create a dragging class?
        glutPostRedisplay();  // cause display quickly
        last_point.setXY(x, y);
    }

    return;
}

void clearView() {
    glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE,
                 window_pixels);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFinish();

    return;
}

// inline int myFloor(float x) { return int(std::floor(x + 0.5)); }

// TODO. what is glutTimerFunc(10, timer, 1);

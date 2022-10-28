// first
#include <GL/glut.h>  // need to be first include
// second
#include <GL/gl.h>  // need to be the second

#include <algorithm>
#include <cmath>
#include <ctime>  // strand
#include <iostream>
#include <stack>

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
std::stack<Poly> poly_stack;
Point last_point;
RGBCode global_rgb;  // global rgb
unsigned char key_mode = 0;
bool dot_drag_mode = 0;
bool reverse_counter = 0;

// my func
void clearView();
void keyModeInit();
inline void storeScreen();  // needed before any drawing option
void reverseScreen();
void polyReverseHandler();
// inline int myFloor(float);

// glut inherit func
void display();
void keyDown(unsigned char key, int x, int y);
void mouseDrag(int x, int y);
void mouse(int bin, int state, int x, int y);

int main(int argc, char* argv[]) {
    // init
    srand(unsigned(time(NULL)));  // random

    // my code
    if(argc!=2) return;
    char& file

    // glut init
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
    // std::cerr << "GLUT_STATE " << state << "\nGLUT_BIN " << bin << std::endl;
    Point now_point(x, y);
    // std::cerr << key_mode << std::endl;

    switch (key_mode) {
        case 'd':
        case 'D': {
            if (state == GLUT_DOWN && bin == GLUT_LEFT_BUTTON) {
                dot_drag_mode = 1;
                storeScreen();
                global_rgb = RGBCode();
                Point(x, y, global_rgb, DEFAULT_SIZE).draw();  // init point
                glutPostRedisplay();
            } else if (state == GLUT_UP && bin == GLUT_LEFT_BUTTON)
                dot_drag_mode = 0;
            break;
        }
        case 'l':
        case 'L': {
            if (state == GLUT_DOWN && bin == GLUT_LEFT_BUTTON) {
                line_mode ^= 1;
                if (!line_mode) {
                    storeScreen();
                    Line(last_point, now_point, RGBCode(), DEFAULT_SIZE).draw();
                    glutPostRedisplay();
                }
            }
            break;
        }
        case 'o':
        case 'O': {
            if (state == GLUT_DOWN && bin == GLUT_LEFT_BUTTON) {
                circle_mode ^= 1;
                if (!circle_mode) {
                    storeScreen();
                    Circle(last_point, now_point, RGBCode(), DEFAULT_SIZE)
                        .draw();
                    glutPostRedisplay();
                }
            }
            break;
        }
        case 'p':
        case 'P': {
            if (state == GLUT_DOWN && bin == GLUT_LEFT_BUTTON) {
                if (poly_stack.size() == 1 && poly_stack.top().isEnded())
                    poly_stack.pop();

                if (poly_stack.size() == 0)
                    poly_stack.push(Poly(now_point, RGBCode(), DEFAULT_SIZE));
                else {
                    storeScreen();
                    poly_stack.top().addPoint(now_point);
                    glutPostRedisplay();
                }
            } else if (state == GLUT_DOWN && bin == GLUT_RIGHT_BUTTON) {
                storeScreen();
                poly_stack.top().end();
                glutPostRedisplay();
            }
            break;
        }
        default: {
            if (state == GLUT_DOWN) std::cerr << "Please input a command\n";
            break;
        }
    }

    if (state == GLUT_DOWN && bin == GLUT_LEFT_BUTTON) last_point.setXY(x, y);

    return;
}

void keyDown(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
        case 'Q': {
            glutDestroyWindow(window);  // exit(0)
            break;  // Quest. need this? -> nope, let OpenGL finishes its
                    // cycle
        }
        case 'r':
        case 'R': {  // reverse: need to store the last scene
            reverseScreen();
            if (key_mode == 'p' || key_mode == 'P') {
                // I make "p"'s own handler, so I don't want them to access the
                // init section
                polyReverseHandler();
                return;
            }
            break;
        }
        case 'c':
        case 'C': {
            clearView();
            break;
        }
        case 'd':
        case 'D':
        case 'l':
        case 'L':
        case 'o':
        case 'O':
        case 'p':
        case 'P': {
            if (key_mode == key)  // if same do not init
                return;
            key_mode = key;
            break;
        }
        default: {
            std::cerr << "\"" << key << "\" is not a recognizable command\n";
            return;
        }
    }

    keyModeInit();
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
    storeScreen();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFinish();
    return;
}

void keyModeInit() {
    switch (key_mode) {
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
            if (poly_stack.size() > 0) poly_stack.pop();  // clear the stack
            break;
        }
    }
    return;
}

inline void storeScreen() {
    reverse_counter = 0;
    glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE,
                 window_pixels);
    return;
}

void reverseScreen() {
    // Quest. and also "rr" result to now scene? -> "Yes"
    glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE,
                 tmp_window_pixels);
    glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE,
                 window_pixels);
    glFinish();
    std::copy(tmp_window_pixels,
              tmp_window_pixels + 3 * WINDOW_WIDTH * WINDOW_HEIGHT,
              window_pixels);
    return;
}

void polyReverseHandler() {
    static bool p_is_ended = 0;

    if (poly_stack.empty()) return;

    if (!reverse_counter) {  // first time reversing
        p_is_ended = poly_stack.top().isEnded();
        last_point = poly_stack.top().delLastPoint();
    } else {
        if (p_is_ended)
            poly_stack.top().end();
        else
            poly_stack.top().addPoint(last_point);
    }
    reverse_counter ^= 1;
    return;
}

// inline int myFloor(float x) { return int(std::floor(x + 0.5)); }

// TODO. what is glutTimerFunc(10, timer, 1);

// first
#include <GL/glut.h>  // need to be first include
// second
#include <GL/gl.h>  // need to be the second

#include <algorithm>
#include <cmath>
#include <cstdlib>  // random
#include <ctime>
#include <iostream>
#include <vector>

#define DEFAULT_SIZE 3.0
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

class RGBCode {
    float r;
    float g;
    float b;

   public:
    RGBCode() {
        this->random();
        return;
    }
    RGBCode(float r, float g, float b) : r(r), g(g), b(b) { return; }
    inline void random() {
        this->r = (float)rand() / (float)RAND_MAX;
        this->g = (float)rand() / (float)RAND_MAX;
        this->b = (float)rand() / (float)RAND_MAX;
        return;
    }
    inline float getR() { return this->r; }
    inline float getG() { return this->g; }
    inline float getB() { return this->b; }
};

class Point {
    int x;
    int y;

   public:
    Point() { return; };
    Point(int x, int y) : x(x), y(y) { return; };
    inline void setXY(int x, int y) {
        this->x = x, this->y = y;
        return;
    }
    inline int getX() { return this->x; }
    inline int getY() { return this->y; }
    void draw(RGBCode rgb, float size) {
        glPointSize(GLfloat(size));
        // glClearColor(0.0, 0.0, 0.0, 0.0);
        // glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(rgb.getR(), rgb.getG(), rgb.getB());

        glBegin(GL_POINTS);
        glVertex2i(this->x, WINDOW_HEIGHT - this->y);
        glEnd();
        return;
    }
};

class Line {
    enum LineBase { LINE_BASE_X, LINE_BASE_Y };

    Point start_p;
    Point end_p;

   public:
    // start == end will cause error
    Line() { return; }
    Line(Point start, Point end) : start_p(start), end_p(end) { return; }
    // inline Point getStartPoint() { return this->start_p; }
    // inline Point getEndPoint() { return this->end_p; }
    void draw(RGBCode rgb, float size) {
        int base[2];
        float in_counter[2];
        float m;
        enum LineBase lb;

        if (std::abs(this->end_p.getY() - this->start_p.getY()) >
            std::abs(this->end_p.getX() - this->start_p.getX())) {
            lb = LINE_BASE_Y;
            m = (float)(this->end_p.getX() - this->start_p.getX()) /
                (float)(this->end_p.getY() - this->start_p.getY());

            if (this->start_p.getY() > this->end_p.getY()) {
                base[0] = this->end_p.getY();
                in_counter[0] = this->end_p.getX();
                base[1] = this->start_p.getY();
                in_counter[1] = this->start_p.getX();
            } else {
                base[0] = this->start_p.getY();
                in_counter[0] = this->start_p.getX();
                base[1] = this->end_p.getY();
                in_counter[1] = this->end_p.getX();
            }
        } else {
            lb = LINE_BASE_X;
            m = (float)(this->end_p.getY() - this->start_p.getY()) /
                (float)(this->end_p.getX() - this->start_p.getX());

            if (this->start_p.getX() > this->end_p.getX()) {
                base[0] = this->end_p.getX();
                in_counter[0] = this->end_p.getY();
                base[1] = this->start_p.getX();
                in_counter[1] = this->start_p.getY();
            } else {
                base[0] = this->start_p.getX();
                in_counter[0] = this->start_p.getY();
                base[1] = this->end_p.getX();
                in_counter[1] = this->end_p.getY();
            }
        }

        // cal points
        int b = base[0];
        float ic = in_counter[0];
        do {
            if (lb == LINE_BASE_Y)
                Point(int(std::floor(ic)), b++).draw(rgb, size);
            else if (lb == LINE_BASE_X)
                Point(b++, int(std::floor(ic))).draw(rgb, size);
            ic += m;
        } while (b <= base[1]);
        return;
    }
};

class Circle {
    Point o;
    float r;

   public:
    Circle() { return; }
    Circle(Point o, Point count_r) : o(o) {
        this->r = std::sqrt(std::pow(count_r.getX() - o.getX(), 2) +
                            std::pow(count_r.getY() - o.getY(), 2));
        return;
    }
    void draw(RGBCode rgb, float size) {
        float cutting_point = this->r * std::sqrt(2);

        // clockwise with mirror
        // y-base: 7/4 pi~ 3/4 pi
        for (float y = -cutting_point; y <= cutting_point; y++) {
            float x_before_x0 = std::sqrt((y + r) * (-y + r));
            Point(int(std::floor(x_before_x0 + this->o.getX())),
                  int(std::floor(y + this->o.getY())))
                .draw(rgb, size);
            Point(int(std::floor(-x_before_x0 + this->o.getX())),
                  int(std::floor(y + this->o.getY())))
                .draw(rgb, size);
        }

        // x-base: 3/4 pi~ 1/4 pi
        for (float x = -cutting_point; x <= cutting_point; x++) {
            float y_before_y0 = std::sqrt((x + r) * (-x + r));
            Point(int(std::floor(x + this->o.getX())),
                  int(std::floor(y_before_y0 + this->o.getY())))
                .draw(rgb, size);
            Point(int(std::floor(x + this->o.getX())),
                  int(std::floor(-y_before_y0 + this->o.getY())))
                .draw(rgb, size);
        }

        return;
    }
};

int window;
GLubyte window_pixels[3 * WINDOW_WIDTH * WINDOW_HEIGHT];
GLubyte tmp_window_pixels[3 * WINDOW_WIDTH * WINDOW_HEIGHT];
int line_click_counter = 0;
Point p_first(-1, -1);
Point o_first(-1, -1);
Point last_point;
RGBCode tmp_rgb;  // global rgb
unsigned char key_mode = 0;
bool dot_drag_mode = 0;

// my func
void clearView();

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
    glClearColor(0.0, 0.0, 0.0, 0.0);
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
                    tmp_rgb.random();
                    now_point.draw(tmp_rgb,
                                   DEFAULT_SIZE);  // Line needs 2 points
                    break;
                }
                case 'l':
                case 'L': {
                    if (++line_click_counter % 2 == 0)
                        Line(last_point, now_point).draw(tmp_rgb, DEFAULT_SIZE);
                    else
                        tmp_rgb.random();
                    break;
                }
                case 'o':
                case 'O': {
                    if (o_first.getX() == -1 && o_first.getY() == -1) {
                        o_first.setXY(x, y);
                        tmp_rgb.random();
                    } else {
                        Circle(o_first, now_point).draw(tmp_rgb, DEFAULT_SIZE);
                        o_first.setXY(-1, -1);  // init
                    }
                    break;
                }
                case 'p':
                case 'P': {
                    if (p_first.getX() == -1 && p_first.getY() == -1) {
                        p_first.setXY(x, y);
                        tmp_rgb.random();
                    } else
                        Line(last_point, now_point).draw(tmp_rgb, DEFAULT_SIZE);
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
            if (key_mode == 'p' || key_mode == 'P') {
                Line(last_point, p_first).draw(tmp_rgb, DEFAULT_SIZE);
                p_first.setXY(-1, -1);  // reset p_first
            }
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
            break;                      // perhaps do not need this?
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
        case 'C': {  // TODO.
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
                    line_click_counter = 0;
                    break;
                }
                case 'o':
                case 'O': {
                    o_first.setXY(-1, -1);
                    break;
                }
                case 'p':
                case 'P': {
                    p_first.setXY(-1, -1);
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
        // Point(x, y).draw(DEFAULT_SIZE);
        Line(last_point, Point(x, y)).draw(tmp_rgb, DEFAULT_SIZE);
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

// TODO. what is glutTimerFunc(10, timer, 1);
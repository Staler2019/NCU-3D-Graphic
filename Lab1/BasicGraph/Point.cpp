#include "./Point.h"

#include "./RGBCode.h"
#include "./Util.h"

// first
#include <GL/glut.h>  // need to be first include
// second
#include <GL/gl.h>  // need to be the second

Point::Point() : BasicGraph() { return; }

Point::Point(int x, int y) : x(x), y(y) { return; }

Point::Point(int x, int y, RGBCode rgbc, float size)
    : x(x), y(y), BasicGraph(rgbc, size) {
    return;
}

void Point::draw() {
    glPointSize(GLfloat(this->edge_size));
    // glClearColor(0.0, 0.0, 0.0, 0.0);
    // glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(this->edge_rgbc.getR(), this->edge_rgbc.getG(),
              this->edge_rgbc.getB());

    glBegin(GL_POINTS);
    glVertex2i(this->x, WINDOW_HEIGHT - this->y);
    glEnd();
    return;
}

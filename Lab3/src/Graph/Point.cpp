#include "Point.h"

#include <iostream>

#include "BasicGraph.h"
#include "RGBCode.h"
#include "Util.h"

// first
#include <GL/glut.h>  // need to be first include
// second
#include <GL/gl.h>  // need to be the second

Point::Point() : BasicGraph(GraphType::GRAPH_POINT) { return; }

Point::Point(float x, float y) : x(x), y(y), BasicGraph(GraphType::GRAPH_POINT) { return; }

Point::Point(float x, float y, RGBCode rgbc, float size) : x(x), y(y), BasicGraph(GraphType::GRAPH_POINT, rgbc, size)
{
    return;
}

void Point::setXY(float x, float y)
{
    this->x = x, this->y = y;
    return;
}

float Point::getX() { return this->x; }

float Point::getY() { return this->y; }

void Point::draw()
{
    if (myFloor(this->x) < 0 || myFloor(this->y) < 0) return;

    glPointSize(GLfloat(this->edge_size));
    // glClearColor(0.0, 0.0, 0.0, 0.0);
    // glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(this->rgbc.getR(), this->rgbc.getG(), this->rgbc.getB());

    glBegin(GL_POINTS);
    // glVertex2i(GLint(myFloor(this->x)), GLint(WINDOW_HEIGHT - myFloor(this->y))); // (0,0) at left-top
    glVertex2i(GLint(myFloor(this->x)), GLint(myFloor(this->y)));
    glEnd();
    // glFlush(); // debug
    return;
}

bool isSamePoint(Point p1, Point p2) { return (p1.getX() == p2.getX() && p1.getY() == p2.getY()); }

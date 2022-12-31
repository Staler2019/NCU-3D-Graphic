#include "Shape/Point.h"

#include <iostream>

#include "GLInclude.h"
#include "Math/Util.h"

void Point::draw() const
{
    if (gFloor(this->x) < 0 || gFloor(this->y) < 0) return;

    glPointSize(GLfloat(this->edge_size));
    // glClearColor(0.0, 0.0, 0.0, 0.0);
    // glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(this->rgb.r, this->rgb.g, this->rgb.b);

    glBegin(GL_POINTS);
    glVertex2i(GLint(gFloor(this->x)), GLint(gFloor(this->y)));
    glEnd();
    // glFlush(); // debug
}

std::ostream& operator<<(std::ostream& os, const Point& that)
{
    os << "(" << that.x << ", " << that.y << ")";
    return os;
}

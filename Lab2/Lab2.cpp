// first
#include <GL/glut.h> // need to be first include
// second
#include <GL/gl.h> // need to be the second

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime> // strand
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <utility>

#include "Graph/Circle.h"
#include "Graph/Line.h"
#include "Graph/Point.h"
#include "Graph/Poly.h"
#include "Graph/RGBCode.h"
#include "Graph/Scene.h"
#include "Graph/TransformMatrix.h"
#include "Graph/Util.h"

#define __TEST__

#ifdef __TEST__
#include <sstream>

// redirect outputs to another output stream.
class redirect_outputs {
    std::ostream &myStream;
    std::streambuf *const myBuffer;

  public:
    redirect_outputs(std::ostream &lhs, std::ostream &rhs = std::cout) : myStream(rhs), myBuffer(myStream.rdbuf())
    {
        myStream.rdbuf(lhs.rdbuf());
    }

    ~redirect_outputs()
    {
        myStream.rdbuf(myBuffer);
    }

    redirect_outputs &operator=(const redirect_outputs &) = delete;
};

// redirect output stream to a string.
class capture_outputs {
    std::ostringstream myContents;
    const redirect_outputs myRedirect;

  public:
    capture_outputs(std::ostream &stream = std::cout) : myContents(), myRedirect(myContents, stream)
    {
    }
    std::string contents() const
    {
        return (myContents.str());
    }
};
#endif

// glut var
int window;

// my var
std::ifstream infile;
Scene pend_scene;
TransformMatrix tr_mat;
std::vector<Point> basic_triangle = {Point(0, 1), Point(-1, -1), Point(1, -1)};
std::vector<Point> basic_square = {Point(-1, -1), Point(1, -1), Point(1, 1), Point(-1, 1)};

// my func
void clearScreen();
void commandHandler();

// glut inherit func
void display();

int main(int argc, char *argv[])
{
#ifdef __TEST__
    std::string default_file = "./test/lab2E.in";
    infile.open(default_file);

    std::ofstream log_file("stderr.log", std::ios::binary);
    if (!log_file.is_open()) {
        std::cerr
            << "Could not open log file for writing."
            << std::endl;
        return (EXIT_FAILURE);
    }
    const redirect_outputs _(log_file, std::cerr);
#else
    // read file
    if (argc != 2) {
        std::cerr << "Please input file\n";
        exit(1);
    }

    infile.open(argv[1]);
#endif

    if (infile.fail()) {
        std::cerr << "Input file opening failed";
        exit(1);
    }

    // init
    srand(unsigned(time(NULL))); // random

    // glut init
    glutInit(&argc, argv);
    // system("pause");
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT); // ?
    window = glutCreateWindow("My Lab2 Window!");
    glutSwapBuffers();
    glutDisplayFunc(display);
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT); // ?
    // TODO. what is glutTimerFunc(10, timer, 1);
    commandHandler();
    glutMainLoop();

    infile.close();
    return 0;
}

void display()
{
    glFlush();
    return;
}

void clearScreen()
{
    // storeScreen();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFinish();
    return;
}

void commandHandler()
{
    while (1) {
        std::cout << "Press any key to continue\n";
        fgetc(stdin);

        std::string in_tmp;

        while (infile >> in_tmp) {
            std::cout << in_tmp;

            if (in_tmp[0] == '#')             // comment
                std::getline(infile, in_tmp); // get '\n'
            else if (in_tmp == "reset") {
                std::cout << std::endl;
                tr_mat.reset();
            }
            else if (in_tmp == "translate") {
                float x, y;
                infile >> x >> y;
                std::cout << " " << x << " " << y << std::endl;
                tr_mat.translate(x, y);
            }
            else if (in_tmp == "scale") {
                float x, y;
                infile >> x >> y;
                std::cout << " " << x << " " << y << std::endl;
                tr_mat.scale(x, y);
            }
            else if (in_tmp == "rotate") {
                float deg;
                infile >> deg;
                std::cout << " " << deg << std::endl;
                tr_mat.rotate(deg);
            }
            else if (in_tmp == "clearData") {
                std::cout << std::endl;
                pend_scene.clear();
            }
            else if (in_tmp == "clearScreen") {
                std::cout << std::endl;
                clearScreen();
            }
            else if (in_tmp == "view") {
                float scene_left, scene_right, scene_bottom, scene_top;
                float window_left, window_right, window_bottom, window_top;
                infile >> scene_left >> scene_right >> scene_bottom >> scene_top >> window_left >> window_right >>
                    window_bottom >> window_top;
                std::cout << " " << scene_left << " " << scene_right << " " << scene_bottom << " " << scene_top << " "
                          << window_left << " " << window_right << " " << window_bottom << " " << window_top
                          << std::endl;
                std::cerr << "Start draw a view:--------------------------------------------------------\n";
                pend_scene.show(scene_left, scene_right, scene_bottom, scene_top, window_left, window_right,
                                window_bottom, window_top);
                // glutPostRedisplay();
                // glFinish();
                glFlush();
                break;
            }
            else if (in_tmp == "square") {
                std::cout << std::endl;
                RGBCode tmp_rgbc;
                Poly *tmp_p = new Poly(basic_square, tmp_rgbc,
                                       DEFAULT_SIZE); // allocate dynamically
                std::cout << "R: " << tmp_rgbc.getR() * 256 - 1 << std::endl;
                std::cout << "G: " << tmp_rgbc.getG() * 256 - 1 << std::endl;
                std::cout << "B: " << tmp_rgbc.getB() * 256 - 1 << std::endl;
                tmp_p->addTransform(tr_mat);
                pend_scene.addLayer(tmp_p);
            }
            else if (in_tmp == "triangle") {
                std::cout << std::endl;
                RGBCode tmp_rgbc;
                Poly *tmp_p = new Poly(basic_triangle, RGBCode(), DEFAULT_SIZE);
                std::cout << "R: " << tmp_rgbc.getR() * 256 - 1 << std::endl;
                std::cout << "G: " << tmp_rgbc.getG() * 256 - 1 << std::endl;
                std::cout << "B: " << tmp_rgbc.getB() * 256 - 1 << std::endl;
                tmp_p->addTransform(tr_mat);
                pend_scene.addLayer(tmp_p);
            }
            else if (in_tmp == "end") {
                std::cout << std::endl;
                glutDestroyWindow(window);
                exit(0);
            }

            // std::cout << "\n";
        }
    }

    return;
}

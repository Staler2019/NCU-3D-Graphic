/**
 * Author:          Staler2019
 * Class:           CSIE 4-B
 * Student Number:  108502571
 * Project Name:    Lab3
 * Finish Date:     22/12/TODO.
 */

/*
 * 3D rendering pipeline:
 *              TM            EM  Parallel Projection(pm)  Perspective
 * Divide(Projection) Object Space -> World Space -> Eye Space -> Perspective
 * Space -> Image Space
 *
 *   Window to Viewport
 *          -> Screen Space
 */

// first
#include <GL/glut.h>  // need to be first include
// second
#include <GL/gl.h>  // need to be the second

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>  // strand
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <utility>

#include "Camera.h"
#include "GMath.h"
#include "GObj.h"
#include "Scene.h"
#include "Transform.h"
// #include "dotenv.h"

// #define __TEST__
#define __STDERR_REDIRECT__

#ifdef __STDERR_REDIRECT__

#include <sstream>

// redirect outputs to another output stream.
class redirect_outputs {
    std::ostream &myStream;
    std::streambuf *const myBuffer;

   public:
    redirect_outputs(std::ostream &lhs, std::ostream &rhs = std::cout)
        : myStream(rhs), myBuffer(myStream.rdbuf())
    {
        myStream.rdbuf(lhs.rdbuf());
    }

    ~redirect_outputs() { myStream.rdbuf(myBuffer); }

    redirect_outputs &operator=(const redirect_outputs &) = delete;
};

// redirect output stream to a string.
class capture_outputs {
    std::ostringstream myContents;
    const redirect_outputs myRedirect;

   public:
    capture_outputs(std::ostream &stream = std::cout)
        : myContents(), myRedirect(myContents, stream)
    {
    }

    std::string contents() const { return (myContents.str()); }
};

#endif  // __STDERR_REDIRECT__

// glut var
int window;

// my var
std::ifstream infile;
int win_width, win_height;
Scene scene;  // TODO.
Transform3D tr_tm;
Camera cam;
std::string mesh_folder = "./test_file/Mesh";
std::string default_file = "./test_file/Data/Lab3C.in";
std::string stderr_file = "stderr.log";

// my func
void clearScreen();

void commandHandler();

// glut inherit func
void display();

int main(int argc, char *argv[])
{
#ifdef __STDERR_REDIRECT__
    // stderr redirection
    std::ofstream log_file(stderr_file, std::ios::binary);
    if (!log_file.is_open()) {
        std::cerr << "Could not open log file for writing." << std::endl;
        return (EXIT_FAILURE);
    }
    const redirect_outputs _(log_file, std::cerr);
#endif  // __STDERR_REDIRECT__
#ifdef __TEST__
    infile.open(default_file);
#else   // __TEST__
    // read file
    if (argc != 2) {
        std::cerr << "Please input file\n";
        exit(1);
    }

    infile.open(argv[1]);
#endif  // __TEST__

    if (infile.fail()) {
        std::cerr << "Input file opening failed";
        exit(1);
    }

    // dotenv::env.load_dotenv();
    // mesh_folder = dotenv::env["MESH_FOLDER"];

    // init
    srand(unsigned(time(NULL)));  // random

    infile >> win_width >> win_height;
    std::cerr << "Create window in (" << win_width << ", " << win_height
              << ")\n";
    scene.setWindow(win_width, win_height);

    // glut init
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(win_width, win_height);
    glutInitWindowPosition(100, 100);
    gluOrtho2D(0, win_width, 0, win_height);
    window = glutCreateWindow("My Lab3 Window!");
    glutSwapBuffers();
    glutDisplayFunc(display);
    gluOrtho2D(0, win_width, 0, win_height);

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
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFinish();
    return;
}

void commandHandler()
{
    // std::cout << "Press any key to continue...\n";
    // fgetc(stdin);

    while (1) {
        std::cout << "Press any key to continue...\n";
        fgetc(stdin);

        std::string in_tmp;

        while (infile >> in_tmp) {
            std::cout << in_tmp;

            if (in_tmp[0] == '#') {            // comment
                std::getline(infile, in_tmp);  // get '\n'

                std::cout << " " << in_tmp << "\n";
            }
            else if (in_tmp == "reset") {
                tr_tm.reset();
            }
            else if (in_tmp == "translate") {
                float x, y, z;
                infile >> x >> y >> z;

                std::cout << " " << x << " " << y << " " << z;

                tr_tm.translate(Vector3(x, y, z));
            }
            else if (in_tmp == "scale") {
                float x, y, z;
                infile >> x >> y >> z;

                std::cout << " " << x << " " << y << " " << z;

                tr_tm.scale(Vector3(x, y, z));
            }
            else if (in_tmp == "rotate") {
                float deg_x, deg_y, deg_z;
                infile >> deg_x >> deg_y >> deg_z;

                std::cout << " " << deg_x << " " << deg_y << " " << deg_z;

                tr_tm.rotate(Vector3(deg_x, deg_y, deg_z));
            }
            else if (in_tmp == "clearData") {
                scene.clear();
            }
            else if (in_tmp == "clearScreen") {
                clearScreen();
            }
            else if (in_tmp == "viewport") {
                float vxl, vxr, vyb, vyt;
                infile >> vxl >> vxr >> vyb >> vyt;

                std::cout << " " << vxl << " " << vxr << " " << vyb << " "
                          << vyt;

                scene.setViewport(vxl, vxr, vyb, vyt);
            }
            else if (in_tmp == "object") {
                std::string obj_name;
                infile >> obj_name;

                std::cout << " " << obj_name << "\n";

                GObj *tmp_obj = new GObj(mesh_folder + "/" + obj_name, tr_tm);
                if (!tmp_obj->fail()) {
                    scene.addLayer(tmp_obj);
                }
                else {
                    std::cerr << "Load obj " << obj_name << " error\n";
                    exit(1);
                }
            }
            else if (in_tmp == "observer") {
                float epx, epy, epz, COIx, COIy, COIz, Tilt, Hither, Yon, Hav;
                infile >> epx >> epy >> epz >> COIx >> COIy >> COIz >> Tilt >>
                    Hither >> Yon >> Hav;

                std::cout << " " << epx << " " << epy << " " << epz << " "
                          << COIx << " " << COIy << " " << COIz << " " << Tilt
                          << " " << Hither << " " << Yon << " " << Hav;

                cam.setPosi(Vector3(epx, epy, epz));
                cam.setFromTo(Vector3(COIx - epx, COIy - epy, COIz - epz),
                              Tilt);
                cam.setVision(Hither, Yon, Hav);
            }
            else if (in_tmp == "display") {
                clearScreen();

                std::cout << "\n" << std::endl;
                std::cerr << "Start draw a "
                             "view:--------------------------------------------"
                             "------------\n";

                scene.show(cam);
                glutPostRedisplay();
                glFinish();
                glFlush();
                break;
            }
            else if (in_tmp == "end") {
                std::cout << std::endl;

                glutDestroyWindow(window);
                exit(0);
            }

            std::cout << std::endl;
        }
    }
}

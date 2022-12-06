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

#include "GObj.h"
#include "Camera.h"
#include "GMath.h"
#include "Scene.h"
#include "Transform.h"
// #include "OBJ_Loader.h"
// #include "dotenv.h"

#define __TEST__
// #define __STDERR_REDIRECT__

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

    ~redirect_outputs() { myStream.rdbuf(myBuffer); }

    redirect_outputs &operator=(const redirect_outputs &) = delete;
};

// redirect output stream to a string.
class capture_outputs {
    std::ostringstream myContents;
    const redirect_outputs myRedirect;

   public:
    capture_outputs(std::ostream &stream = std::cout) : myContents(), myRedirect(myContents, stream) {}
    std::string contents() const { return (myContents.str()); }
};
#endif  // __TEST__

// glut var
int window;

// my var
std::ifstream infile;
int win_width, win_height;
Scene scene;  // TODO.
Transform3D tr;
Camera cam;
std::string mesh_folder = "./test_file/Mesh";

// my func
void clearScreen();
void commandHandler();

// glut inherit func
void display();

int main(int argc, char *argv[])
{
#ifdef __TEST__
    std::string default_file = "./test_file/Data/Lab3A.in";
    infile.open(default_file);

#ifdef __STDERR_REDIRECT__
    // stderr redirection
    std::ofstream log_file("stderr.log", std::ios::binary);
    if (!log_file.is_open()) {
        std::cerr << "Could not open log file for writing." << std::endl;
        return (EXIT_FAILURE);
    }
    const redirect_outputs _(log_file, std::cerr);
#endif // __STDERR_REDIRECT__
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
    std::cerr << "Create window in (" << win_width << ", " << win_height << ")\n";
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
    std::cout << "Press any key to continue...\n";
    fgetc(stdin);

    while (1) {
        std::cout << "Press any key to continue...\n";
        fgetc(stdin);

        std::string in_tmp;

        while (infile >> in_tmp) {
            std::cout << in_tmp;

            if (in_tmp[0] == '#')              // comment
                std::getline(infile, in_tmp);  // get '\n'
            else if (in_tmp == "reset") {
                std::cout << std::endl;
                tr.reset();
            }
            else if (in_tmp == "translate") {
                float x, y, z;
                infile >> x >> y >> z;
                std::cout << " " << x << " " << y << " " << z << std::endl;
                tr.translate(Vector3(x, y, z));
            }
            else if (in_tmp == "scale") {
                float x, y, z;
                infile >> x >> y >> z;
                std::cout << " " << x << " " << y << " " << z << std::endl;
                tr.scale(Vector3(x, y, z));
            }
            else if (in_tmp == "rotate") {
                float deg_x, deg_y, deg_z;
                infile >> deg_x >> deg_y >> deg_z;
                std::cout << " " << deg_x << " " << deg_y << " " << deg_z << std::endl;
                tr.rotate(Vector3(deg_x, deg_y, deg_z));
            }
            else if (in_tmp == "clearData") {
                std::cout << std::endl;
                scene.clear();
            }
            else if (in_tmp == "clearScreen") {
                std::cout << std::endl;
                clearScreen();
            }
            else if (in_tmp == "viewport") {
                float vxl, vxr, vyb, vyt;
                infile >> vxl >> vxr >> vyb >> vyt;
                std::cout << " " << vxl << " " << vxr << " " << vyb << " " << vyt << std::endl;

                scene.setViewport(vxl, vxr, vyb, vyt);
            }
            else if (in_tmp == "object") {
                std::string obj_name;
                infile >> obj_name;
                std::cout << " " << obj_name << std::endl;

                GObj *tmp_obj = new GObj(mesh_folder + "/" + obj_name, tr);
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
                infile >> epx >> epy >> epz >> COIx >> COIy >> COIz >> Tilt >> Hither >> Yon >> Hav;
                std::cout << " " << epx << " " << epy << " " << epz << " " << COIx << " " << COIy << " " << COIz << " "
                          << Tilt << " " << Hither << " " << Yon << " " << Hav << std::endl;

                cam.setPosi(Vector3(epx, epy, epz));
                cam.setFromTo(Vector3(COIx, COIy, COIz), Tilt);
                cam.setVision(Hither, Yon, Hav);
            }
            else if (in_tmp == "display") {
                clearScreen();
                std::cerr << "Start draw a view:--------------------------------------------------------\n";
                scene.show(cam);
                // glutPostRedisplay();
                // glFinish();
                glFlush();
                break;
            }
            else if (in_tmp == "end") {
                std::cout << std::endl;
                glutDestroyWindow(window);
                exit(0);
            }

            std::cout << "\n";
        }
    }
}
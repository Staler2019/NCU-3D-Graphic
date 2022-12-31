/**
 * Author:          Staler2019
 * Class:           CSIE 4-B
 * Student Number:  108502571
 * Project Name:    Lab3
 * Finish Date:     22/12/TODO.
 */

/*
 * 3D rendering pipeline:
 *              TM             EM           pm
 * Object Space -> World Space -> Eye Space -> Perspective Space
 *
 *   Perspective Divide  Window to Viewport
 *          -> Image Space -> Screen Space
 */

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
#include "GLInclude.h"
#include "GMath.h"
#include "GObj.h"
#include "Light.h"
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
    capture_outputs(std::ostream &stream = std::cout) : myContents(), myRedirect(myContents, stream)
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
Scene3D scene;
Transform3D tr_tm;
Camera cam;
std::string mesh_folder = "./Mesh/";
std::string data_folder = "./Data/";
std::string default_file = "./Data/Lab4D.in";
std::string stderr_file = "stderr.log";

// my func
void clearScreen();
void commandHandler();
// void screenShot();

// glut inherit func
void display();
// void keyDown(unsigned char key, int x, int y);

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

    infile.open(data_folder + std::string(argv[1]));
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
    window = glutCreateWindow("108502571 Lab4!");
    glutSwapBuffers();
    glutDisplayFunc(display);
    // glutKeyboardFunc(keyDown);
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

                tr_tm.rotate(Vector3(toRad(deg_x), toRad(deg_y), toRad(deg_z)));
            }
            else if (in_tmp == "cleardata") {
                scene.clear();
            }
            else if (in_tmp == "clearScreen") {
                clearScreen();
            }
            else if (in_tmp == "viewport") {
                float vxl, vxr, vyb, vyt;
                infile >> vxl >> vxr >> vyb >> vyt;

                std::cout << " " << vxl << " " << vxr << " " << vyb << " " << vyt;

                scene.setViewport(vxl, vxr, vyb, vyt);
            }
            else if (in_tmp == "object") {
                std::string obj_name;
                float Or, Og, Ob, Kd, Ks;
                int N;
                infile >> obj_name >> Or >> Og >> Ob >> Kd >> Ks >> N;

                std::cout << " " << obj_name << " " << Or << " " << Og << " " << Ob << " " << Kd
                          << " " << Ks << " " << N << "\n";

                GObj *tmp_obj =
                    new GObj(mesh_folder + obj_name, tr_tm, GRGB(Or, Og, Ob), Kd, Ks, N);
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

                std::cout << " " << epx << " " << epy << " " << epz << " " << COIx << " " << COIy
                          << " " << COIz << " " << Tilt << " " << Hither << " " << Yon << " "
                          << Hav;

                cam.setPosi(Vector3(epx, epy, epz));
                cam.setFromTo(Vector3(COIx - epx, COIy - epy, COIz - epz), toRad(Tilt));
                cam.setVision(Hither, Yon, toRad(Hav));
            }
            else if (in_tmp == "display") {
                // clearScreen(); // already change buffer in showTextures

                std::cout << "\n" << std::endl;
                std::cerr << "Start draw a "
                             "view:--------------------------------------------"
                             "------------\n";

                scene.showTextures(cam);
                // scene.showLines(cam);
                // glutPostRedisplay();
                // glFinish();
                glFlush();
                break;
            }
            else if (in_tmp == "ambient") {
                float KIr, KIg, KIb;
                infile >> KIr >> KIg >> KIb;
                std::cout << " " << KIr << " " << KIg << " " << KIb;

                scene.setAmbientRGB(GRGB(KIr, KIg, KIb));
            }
            else if (in_tmp == "background") {
                float Br, Bg, Bd;
                infile >> Br >> Bg >> Bd;
                std::cout << " " << Br << " " << Bg << " " << Bd;

                scene.setBGRGB(GRGB(Br, Bg, Bd));
            }
            else if (in_tmp == "light") {
                int idx;
                float Ipr, Ipg, Ipb, Ix, Iy, Iz;
                infile >> idx >> Ipr >> Ipg >> Ipb >> Ix >> Iy >> Iz;
                std::cout << " " << idx << " " << Ipr << " " << Ipg << " " << Ipb << " " << Ix
                          << " " << Iy << " " << Iz;

                scene.setLight(idx, Light(Vector3(Ix, Iy, Iz), GRGB(Ipr, Ipg, Ipb)));
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

// void keyDown(unsigned char key, int x, int y)
// {
//     switch (key) {
//         case 's':
//         case 'S': {
//             screenShot();
//             break;
//         }
//     }
// }

// void screenShot()
// {
//     time_t now = time(0);
//     char dt[128];
//     errno_t err_time = ctime_s(dt, sizeof dt, &now);

//     std::string f_name = dt;
//     std::replace(f_name.begin(), f_name.end(), ' ', '-');
//     f_name += ".png";

//     int &dimx = win_width;
//     int &dimy = win_height;

//     size_t imsize = 3 * dimx * dimy;
//     char *pixels = (char *)malloc(imsize * sizeof(char));
//     glReadPixels(0, 0, dimx, dimy, GL_RGB, GL_UNSIGNED_BYTE, pixels);

//     FILE *fp;
//     errno_t err = fopen_s(&fp, f_name.c_str(), "wb");
//     fprintf(fp, "P6\n%d %d\n255\n", dimx, dimy);
//     fwrite(pixels, sizeof(char), imsize, fp);
//     fclose(fp);

//     free(pixels);
// }

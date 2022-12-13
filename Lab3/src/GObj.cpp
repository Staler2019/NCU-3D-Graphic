#include "GObj.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "Math/Vector.h"

void GObj::load_thread_job()
{
    this->thread_lock = true;

    this->loaded_faces.clear();
    this->loaded_vertexes.clear();

    std::ifstream infile(this->file_name);
    std::string str;
    int pos;
    std::vector<std::string> para;

    if (infile.fail()) {
        this->loaded = 0;
        return;
    }

    while (getline(infile, str) && str != "^Z") {
        para.clear();
        if (str.length() == 0) continue;
        if (str[0] == '#') continue;

        // std::cout << str << std::endl;
        pos = str.find(' ');
        while (pos != std::string::npos) {
            para.push_back(str.substr(0, pos));
            int l = 0;
            char stupidVS = ' ';
            for (int i = pos; str[i] == stupidVS; i++, l++)
                ;
            str.erase(str.begin(), str.begin() + pos + l);
            pos = str.find(" ");
        }
        if (str.length() != 0 && str[0] != '\r') para.push_back(str);
        // for (int i = 0; i < para.size(); i++)
        //     std::cout << para[i] << " ";
        // std::cout << std::endl;
        if (para[0] == "mtllib" || para[0] == "usemtl")
            continue;
        else if (para[0] == "v") {
            Vector3 p(atof(para[1].c_str()), atof(para[2].c_str()),
                      atof(para[3].c_str()));
            this->loaded_vertexes.push_back(p);
        }
        else if (para[0] == "f") {  // may change loadfaces into function that
                                    // access loadvertices
            std::vector<Vector3> vs;
            // std::cout << "f" << LoadedFaces.size() << ": " << para.size() <<
            // std::endl;
            for (int i = 1; i < para.size(); i++) {
                pos = para[i].find('/');
                if (pos != std::string::npos) {
                    vs.push_back(
                        this->loaded_vertexes
                            [atoi(para[i].substr(0, pos).c_str()) - 1]);
                }
                else {
                    vs.push_back(
                        this->loaded_vertexes[atoi(para[i].c_str()) - 1]);
                }
            }  // std::cout << std::endl;
            Face f(vs);
            this->loaded_faces.push_back(f);
            /*for (int i = 0; i < f.Vertices.size(); i++) {
                std::cout << f.Vertices[i].Position.X << " ";
                std::cout << f.Vertices[i].Position.Y << " ";
                std::cout << f.Vertices[i].Position.Z << std::endl;
            }*/
        }
    }
    infile.close();
    this->loaded = 1;

    this->thread_lock = false;
}

void GObj::load()
{
    std::cout << "Wait for obj loading...\n";

    if (!this->thread_lock) {
        std::thread loading_thread(&GObj::load_thread_job, this);
        loading_thread.join();
    }
    else {
        std::cerr << this->file_name
                  << " is loaded...please wait for Pre. Op.\n";
    }
}
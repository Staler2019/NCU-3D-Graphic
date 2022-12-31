#include "GObj.h"

#include <fstream>
#include <iostream>
#include <vector>

#include "Math/Vector.h"
#include "Util.h"

void GObj::clear()
{
    // for (Face& f : this->faces) {
    //     for (V* v : f.Vs) v = nullptr;
    //     for (VT* vt : f.VTs) vt = nullptr;
    //     for (VN* vn : f.VNs) vn = nullptr;

    //     f.Vs.clear();
    //     f.VTs.clear();
    //     f.VNs.clear();
    // }

    this->faces.clear();
    this->Vs.clear();
    this->VTs.clear();
    this->VNs.clear();
}

void GObj::load_thread_job()
{
    this->thread_lock = true;

    this->clear();

    {
        std::ifstream infile(this->file_name);

        if (infile.fail()) {
            this->loaded = 0;
            return;
        }

        // std::cerr << "Loading " << this->file_name << "\n";

        std::string tmp_in;

        while (getline(infile, tmp_in)) {
            std::vector<std::string> tokens = tokenize(tmp_in, ' ');

            if (tokens.empty()) continue;

            // std::cerr << tokens.size() << "\n";

            // for (auto& token : tokens) {
            //     std::cerr << token << " ";
            // }
            // std::cerr << "\n";

            if (tokens[0] == "v") {
                if (tokens.size() == 3)
                    this->Vs.emplace_back(stof(tokens[1]), stof(tokens[2]), 0);
                else if (tokens.size() == 4)
                    this->Vs.emplace_back(stof(tokens[1]), stof(tokens[2]),
                                          stof(tokens[3]));
            }
            else if (tokens[0] == "vt") {
                if (tokens.size() == 3)
                    this->VTs.emplace_back(stof(tokens[1]), stof(tokens[2]), 0);
                else if (tokens.size() == 4)
                    this->VTs.emplace_back(stof(tokens[1]), stof(tokens[2]),
                                           stof(tokens[3]));
            }
            else if (tokens[0] == "vn") {
                if (tokens.size() == 3)
                    this->VNs.emplace_back(stof(tokens[1]), stof(tokens[2]), 0);
                else if (tokens.size() == 4)
                    this->VNs.emplace_back(stof(tokens[1]), stof(tokens[2]),
                                           stof(tokens[3]));
            }
            else if (tokens[0] == "f") {
                std::vector<V*> tmp_Vs;
                std::vector<VT*> tmp_VTs;
                std::vector<VN*> tmp_VNs;

                for (int i = 1; i < tokens.size(); i++) {
                    std::vector<std::string> tmp_str_tokens =
                        tokenize(tokens[i], '/');

                    std::vector<int> tmp_index_tokens;

                    for (std::string& tmp_str : tmp_str_tokens) {
                        if (tmp_str.size() == 0)
                            tmp_index_tokens.emplace_back(-1);
                        else
                            tmp_index_tokens.emplace_back(stoi(tmp_str) - 1);
                    }

                    if (tmp_index_tokens.size() > 0) {
                        V* tmp_v = (tmp_index_tokens[0] != -1)
                                       ? &this->Vs[tmp_index_tokens[0]]
                                       : nullptr;
                        tmp_Vs.emplace_back(tmp_v);
                    }
                    else
                        tmp_Vs.push_back(nullptr);

                    if (tmp_index_tokens.size() > 1) {
                        VT* tmp_vt = (tmp_index_tokens[1] != -1)
                                         ? &this->VTs[tmp_index_tokens[1]]
                                         : nullptr;
                        tmp_VTs.emplace_back(tmp_vt);
                    }
                    else
                        tmp_VTs.push_back(nullptr);

                    if (tmp_index_tokens.size() > 2) {
                        VN* tmp_vn = (tmp_index_tokens[2] != -1)
                                         ? &this->VNs[tmp_index_tokens[2]]
                                         : nullptr;
                        tmp_VNs.emplace_back(tmp_vn);
                    }
                    else
                        tmp_VNs.push_back(nullptr);
                }

                this->faces.emplace_back(tmp_Vs, tmp_VTs, tmp_VNs);
            }
        }

        // while (getline(infile, str) && str != "^Z") {
        //     para.clear();
        //     if (str.length() == 0) continue;
        //     if (str[0] == '#') continue;

        //     // std::cout << str << std::endl;
        //     pos = str.find(' ');
        //     while (pos != std::string::npos) {
        //         para.push_back(str.substr(0, pos));
        //         int l = 0;
        //         char stupidVS = ' ';
        //         for (int i = pos; str[i] == stupidVS; i++, l++)
        //             ;
        //         str.erase(str.begin(), str.begin() + pos + l);
        //         pos = str.find(" ");
        //     }
        //     if (str.length() != 0 && str[0] != '\r') para.push_back(str);
        //     // for (int i = 0; i < para.size(); i++)
        //     //     std::cout << para[i] << " ";
        //     // std::cout << std::endl;
        //     if (para[0] == "mtllib" || para[0] == "usemtl")
        //         continue;
        //     else if (para[0] == "v") {
        //         Vector3 p(atof(para[1].c_str()), atof(para[2].c_str()),
        //                   atof(para[3].c_str()));
        //         this->loaded_vertexes.push_back(p);
        //     }
        //     else if (para[0] == "f") {  // may change loadfaces into
        //                                 // function that access loadvertices
        //         std::vector<Vector3> vs;
        //         // std::cout << "f" << LoadedFaces.size() << ": " <<
        //         // para.size()
        //         // << std::endl;
        //         for (int i = 1; i < para.size(); i++) {
        //             pos = para[i].find('/');
        //             if (pos != std::string::npos) {
        //                 vs.push_back(
        //                     this->loaded_vertexes
        //                         [atoi(para[i].substr(0, pos).c_str()) - 1]);
        //             }
        //             else {
        //                 vs.push_back(
        //                     this->loaded_vertexes[atoi(para[i].c_str()) -
        //                     1]);
        //             }
        //         }  // std::cout << std::endl;
        //         Face f(vs);
        //         this->loaded_faces.push_back(f);
        //         /*for (int i = 0; i < f.Vertices.size(); i++) {
        //             std::cout << f.Vertices[i].Position.X << " ";
        //             std::cout << f.Vertices[i].Position.Y << " ";
        //             std::cout << f.Vertices[i].Position.Z << std::endl;
        //         }*/
        //     }
        // }
        infile.close();
    }

    this->loaded = 1;
    this->thread_lock = false;
}

void GObj::load()
{
    std::cout << "Wait for obj loading...\n";

    if (!this->thread_lock) {
        // this->loading_thread = new std::thread(&GObj::load_thread_job, this);
        this->load_thread_job();
        return;
    }
    else {
        std::cerr << this->file_name
                  << " is loaded...please wait for Pre. Op.\n";
        return;
    }
}

#include "GObj.h"

#include <thread>

void GObj::load_thread_job()
{
    this->loading_lock = true;
    this->loading_ok = this->LoadFile(this->file_name);
    this->loading_lock = false;
}

void GObj::load()
{
    std::cout << "Wait for obj loading...\n";
    std::thread loading_thread(&GObj::load_thread_job, this);
    loading_thread.join();
}